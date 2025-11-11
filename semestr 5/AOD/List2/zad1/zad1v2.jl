# Jędrzej Sajnóg 279701

using JuMP
using HiGHS

if length(ARGS) == 0
    throw("error no input file")
end

function get_input_values(filename)
    company_supplies = []
    airport_demands = []
    open(filename, "r") do file
        ncomp = parse(Int64,readline(file))
        nair = parse(Int64, readline(file))
        costs = zeros(Int64, nair, ncomp)
        for i in 1:ncomp
            c = parse(Int64, readline(file))
            push!(company_supplies, c)
        end
        for i in 1:nair
            a = parse(Int64, readline(file))
            push!(airport_demands, a)
        end
        for i in 1:nair
            for j in 1:ncomp
                costs[i, j] = parse(Int64, readline(file))
            end
        end
        return ncomp, nair, company_supplies, airport_demands, costs
    end
end


ncomp, nair, company_supplies, airport_demands, costs = get_input_values(ARGS[1])


println(ncomp)
println(nair)

# ncomp = length(company_supplies)
# nair  = length(airport_demands)

model = Model(HiGHS.Optimizer)

# indexed variables x[company, airport] >= 0 (continuous)
@variable(model, x[1:ncomp, 1:nair] >= 0)

# objective: minimize total cost
@objective(model, Min, sum(costs[a, c] * x[c, a] for c in 1:ncomp, a in 1:nair))

# supply limits (per company)
for c in 1:ncomp
    @constraint(model, sum(x[c, a] for a in 1:nair) <= company_supplies[c])
end

# demand constraints (per airport) -- exact demand
for a in 1:nair
    @constraint(model, sum(x[c, a] for c in 1:ncomp) == airport_demands[a])
end

optimize!(model)

println("termination status: ", termination_status(model))
println("objective (total money): ", objective_value(model))
for c in 1:ncomp
    for a in 1:nair
        println("company $c -> airport $a: ", value(x[c,a]))
    end
end
