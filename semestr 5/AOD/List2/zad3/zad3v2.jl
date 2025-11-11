using HiGHS
using JuMP


function get_input_values(filename)
    c = []
    a = []
    o_cost = []
    d = []
    open(filename, "r") do file
        K = parse(Int64,readline(file))
        normal_cap = parse(Int64, readline(file))
        storage_cost = parse(Int64, readline(file))
        storage_cap = parse(Int64, readline(file))
        s0 = parse(Int64, readline(file))
        for i in 1:K
            v = parse(Int64, readline(file))
            push!(c, v)
        end
        for i in 1:K
            v = parse(Int64, readline(file))
            push!(a, v)
        end
        for i in 1:K
            v = parse(Int64, readline(file))
            push!(o_cost, v)
        end
        for i in 1:K
            v = parse(Int64, readline(file))
            push!(d, v)
        end
        return K, normal_cap, storage_cost, storage_cap, s0, c, a, o_cost, d
    end
end


# dane
# K = 4
# c = [6000.0, 4000.0, 8000.0, 9000.0]   # koszt normalnej produkcji cj
# a = [60.0, 65.0, 70.0, 60.0]          # maksymalna produkcja ponadwymiarowa aj
# o_cost = [8000.0, 6000.0, 10000.0, 11000.0] # koszt overtime oj
# d = [130.0, 80.0, 125.0, 195.0]       # zapotrzebowanie dj

# normal_cap = 100.0
# storage_cost = 1500.0
# storage_cap = 70.0
# s0 = 15.0


K, normal_cap, storage_cost, storage_cap, s0, c, a, o_cost, d = get_input_values(ARGS[1])




model = Model(HiGHS.Optimizer)

@variable(model, 0 <= p[1:K] <= normal_cap)
@variable(model, 0 <= ot[1:K])
set_upper_bound.(ot, a)
@variable(model, 0 <= s[1:K] <= storage_cap)

@constraint(model, s[1] == s0 + p[1] + ot[1] - d[1])
for i in 2:K
    @constraint(model, s[i] == s[i-1] + p[i] + ot[i] - d[i])
end

@objective(model, Min, sum(p .* c) + sum(ot .* o_cost) + storage_cost * sum(s))

optimize!(model)
println("termination status: ", termination_status(model))
println("objective (total money): ", objective_value(model))
for i in 1:K
    println("timestep: $i, produced: $(value(p[i])), overtimeL $(value(ot[i])), stored: $(value(s[i])), demand: $(d[i])")
end