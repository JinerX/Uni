using HiGHS
using JuMP

if length(ARGS) == 0
    throw("error no input file")
end


# txt file structure
# 
# n_products
# n_machines
# demand for prod 1
# demand for prod 2
# ...
# demand for prod k
# production cost of prod 1
# production cost of prod 2
# ...
# production cost of prod k
# selling cost of prod 1
# selling cost of prod 2
# ...
# selling cost of prod k
# supply for machine 1 in hours
# supply for machine 2 in hours
# ...
# supply for machine n in hours
# cost of using machine 1 for 1 hour
# cost of using machine 2 for 1 hour
# ...
# cost of using machine n for 1 hour
# time prod 1 takes in machine 1 in minutes
# time prod 1 takes in machine 2 in minutes
# ...
# time prod 1 takes in machine n in minutes
# time prod 2 takes in machine 1 in minutes
# ...
# time prod k takes in machine n in minutes 


function get_input_values(filename)
    product_demands = []
    machine_supplies = []
    product_production_costs = []
    product_selling_costs = []
    machine_work_costs = []
    open(filename, "r") do file
        n_products = parse(Int64,readline(file))
        n_machines = parse(Int64, readline(file))
        product_machine_time = zeros(Int64, n_products, n_machines)
        for i in 1:n_products
            c = parse(Int64, readline(file))
            push!(product_demands, c)
        end
        for i in 1:n_products
            c = parse(Int64, readline(file))
            push!(product_production_costs, c)
        end
        for i in 1:n_products
            c = parse(Int64, readline(file))
            push!(product_selling_costs, c)
        end
        for i in 1:n_machines
            a = parse(Int64, readline(file))
            push!(machine_supplies, a)
        end
        for i in 1:n_machines
            a = parse(Int64, readline(file))
            push!(machine_work_costs, a)
        end
        for i in 1:n_products
            for j in 1:n_machines
                product_machine_time[i, j] = parse(Int64, readline(file))
            end
        end
        return n_products, n_machines, product_demands, product_production_costs, product_selling_costs, machine_supplies, machine_work_costs, product_machine_time
    end
end

function print_input_vals(n_products, n_machines, product_demands, product_production_costs, product_selling_costs, machine_supplies, machine_work_costs, product_machine_time)
    println("n_products: $n_products")
    println("n_machines: $n_machines")
    println("product_demands: $product_demands")
    println("product_production_costs: $product_production_costs")
    println("product_selling_costs: $product_selling_costs")
    println("machine_supplies: $machine_supplies")
    println("machine qork costs: $machine_work_costs")
    println("product_machine_time: $product_machine_time")
end


n_products, n_machines, product_demands, product_production_costs, product_selling_costs, machine_supplies, machine_work_costs, product_machine_time = get_input_values(ARGS[1])

print_input_vals(n_products, n_machines, product_demands, product_production_costs, product_selling_costs, machine_supplies, machine_work_costs, product_machine_time)

println(ones(Int64, 4) .* product_selling_costs)


model = Model(HiGHS.Optimizer)
@variable(model, x[1:n_products] >= 0)
@objective(model, Max,
    sum(product_selling_costs[p] * x[p] for p in 1:n_products)
  - sum(product_production_costs[p] * x[p] for p in 1:n_products)
  - sum(machine_work_costs[j] * sum(product_machine_time[p,j] * x[p] for p in 1:n_products) / 60
        for j in 1:n_machines)
)

# constraints


# constraints over demand
for p in 1:n_products
    @constraint(model, x[p] <= product_demands[p])
end

# constraints over machine time
for j in 1:n_machines
    @constraint(model,
        sum(product_machine_time[p,j] * x[p] for p in 1:n_products) / 60 <= machine_supplies[j]
    )
end

optimize!(model)
println("termination status: ", termination_status(model))
println("profit: ", objective_value(model))
for c in 1:n_products
    println("product: $c: ", value(x[c]))
end

