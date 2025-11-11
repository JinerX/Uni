# Jędrzej Sajnóg 279701

using JuMP
using HiGHS

decision_cariables = []
number_of_companies = 3
number_of_airports = 4
company_supplies = []



model = Model(HiGHS.Optimizer)
# defining variables

# company 1
F1L1 = @variable(model, 0 <= F1L1)
F1L2 = @variable(model, 0 <= F1L2)
F1L3 = @variable(model, 0 <= F1L3)
F1L4 = @variable(model, 0 <= F1L4)

# company 2
F2L1 = @variable(model, 0 <= F2L1)
F2L2 = @variable(model, 0 <= F2L2)
F2L3 = @variable(model, 0 <= F2L3)
F2L4 = @variable(model, 0 <= F2L4)

#company 3
F3L1 = @variable(model, 0 <= F3L1)
F3L2 = @variable(model, 0 <= F3L2)
F3L3 = @variable(model, 0 <= F3L3)
F3L4 = @variable(model, 0 <= F3L4)

# defining the cost function

@objective(model, Min, F1L1 + F1L2 + F1L3 + F1L4 + F2L1 + F2L2 + F2L3 + F2L4 + F3L1 + F3L2 + F3L3 + F3L4)




# defining constraints

# over maximum supply
F1_supply = @constraint(model, F1_supply, F1L1 + F1L2 + F1L3 + F1L4 <= 275000)
F2_supply = @constraint(model, F2_supply, F2L1 + F2L2 + F2L3 + F2L4 <= 550000)
F3_supply = @constraint(model, F3_supply, F3L1 + F3L2 + F3L3 + F3L4 <= 660000)

# over airport needs
L1_demand= @constraint(model, L1_demand, F1L1 + F2L1 + F3L1 >= 110000)
L2_demand= @constraint(model, L2_demand, F1L2 + F2L2 + F3L2 >= 220000)
L3_demand= @constraint(model, L3_demand, F1L3 + F2L3 + F3L3 >= 330000)
L4_demand= @constraint(model, L4_demand, F1L4 + F2L4 + F3L4 >= 440000)


println("model:\n $model")

optimize!(model)

println("termination status:\n $(termination_status(model))")
println("primal status:\n $(primal_status(model))")
println("objective function: $(objective_value(model))")
println("F1 variables: L1: $(value(F1L1)), L2: $(value(F1L2)), L3: $(value(F1L3)), L4: $(value(F1L4))")
println("F2 variables: L1: $(value(F2L1)), L2: $(value(F2L2)), L3: $(value(F2L3)), L4: $(value(F2L4))")
println("F3 variables: L1: $(value(F3L1)), L2: $(value(F3L2)), L3: $(value(F3L3)), L4: $(value(F3L4))")
