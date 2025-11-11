# Jędrzej Sajnóg 279701


using Plots

gr()

n = 40

input_vals = [
    #c, x0 
    (-2,1),
    (-2,2),
    (-2,1.99999999999999),
    (-1,1),
    (-1,-1),
    (-1,0.75),
    (-1,0.25)
]


function simulate_array(x0,n,c)
    results = []
    x_tmp = x0
    push!(results, x_tmp)
    for i in 1:1:n
        println("$(i)th element: $x_tmp")
        x_tmp = x_tmp^2 + c
        push!(results, x_tmp)
    end
    println("$(n+1)th element: $x_tmp")
    return results
end


for (c,x0) in input_vals
    println("SIMULATION: c:$c, x0: $x0")
    # simulate(x0,n,c)
    arr = simulate_array(x0,n,c)
    p = plot(1:1:(n+1), arr, label="c:$c, x0:$x0")
    savefig(p, "c:$c, x0:$x0.png")
    gui()
    readline()
end