# Jędrzej Sajnóg 279701

include("../interpolation_module/interpolation_module.jl")
using .InterpolationModule
const IM = InterpolationModule

using Plots


x_generation = [:rownoodlegle, :czebyszew]
ns = [5,10,15]

f1(x) = abs(x)
f2(x) = 1/(1+x^2)
for symbol in x_generation
    println("Symbol: $symbol")
    for n in ns
        println("n: $n")
        p1 = IM.rysujNnfx(f1, -1.0,1.0,n,100, symbol)
        savefig(p1, "func: |x|, n:$n, type:$symbol.png")
        gui()
        readline()
        p2 = IM.rysujNnfx(f2, -5.0,5.0, n,100, symbol)
        savefig(p2, "func:1_1+x^2), n:$n, type: $symbol.png")
        gui()
        readline()
    end
end