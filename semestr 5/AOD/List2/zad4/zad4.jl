using JuMP
using HiGHS

if length(ARGS) == 0
    throw("error no input file")
end

# Linia 1: Liczba wierzchołków (N)
# Linia 2: Wierzchołek startowy (s)
# Linia 3: Wierzchołek końcowy (t)
# Linia 4: Maksymalny dopuszczalny czas (T)
# Linia 5: Liczba krawędzi (ne)
# Kolejne linie (ne): u v koszt czas (dla każdej krawędzi)


function get_input_values(filename)
    arcs = []

    open(filename, "r") do file
        # parse constant values
        N = parse(Int64, readline(file))
        s = parse(Int64, readline(file))
        t = parse(Int64, readline(file))
        T = parse(Int64, readline(file))
        n_arcs = parse(Int64, readline(file)) # number of arcs to load

        # load each arc
        for _ in 1:n_arcs
            line = readline(file)
            parts = split(line)
            u = parse(Int64, parts[1])
            v = parse(Int64, parts[2])
            cost = parse(Int64, parts[3])
            time = parse(Int64, parts[4])

            push!(arcs, (u, v, cost, time))
        end

        return N, s, t, T, arcs
    end
end



# dane grafu: (u, v, cost, time)
arcs = [
    (1, 2, 3, 4),  (1, 3, 4, 9),  (1, 4, 7, 10), (1, 5, 8, 12),
    (2, 3, 2, 3),
    (3, 4, 4, 6),  (3, 5, 2, 2),  (3, 10, 6, 11),
    (4, 5, 1, 1),  (4, 7, 3, 5),
    (5, 6, 5, 6),  (5, 7, 3, 3),  (5, 10, 5, 8),
    (6, 1, 5, 8),  (6, 7, 2, 2),  (6, 10, 7, 11),
    (7, 3, 4, 6),  (7, 8, 3, 5),  (7, 9, 1, 1),
    (8, 9, 1, 2),
    (9, 10, 2, 2)
]

N = 10
nodes = 1:N
s = 1
t = 10
T = 15

N, s, t, T, arcs = get_input_values(ARGS[1])


ne = length(arcs)

model = Model(HiGHS.Optimizer)

@variable(model, x[1:ne], Bin)

# minimizing the cost
@objective(model, Min, sum(arcs[e][3] * x[e] for e = 1:ne))

# time constraint
@constraint(model, sum(arcs[e][4] * x[e] for e = 1:ne) <= T)

# circulation constraint
for v in nodes
    out_sum = sum(x[e] for e = 1:ne if arcs[e][1] == v; init = 0)
    in_sum  = sum(x[e] for e = 1:ne if arcs[e][2] == v; init = 0)
    rhs = v == s ? 1 : v == t ? -1 : 0
    @constraint(model, out_sum - in_sum == rhs)
end

optimize!(model)

println("termination status: ", termination_status(model))
println("objective total cost: ", objective_value(model))
println("path: ")
for i in 1:ne
    if value(x)[i] == 1
        println("$(arcs[i])")
    end
end
println("end of path")
