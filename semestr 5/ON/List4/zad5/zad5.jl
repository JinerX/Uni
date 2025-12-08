# Jędrzej Sajnóg 279701

include("../interpolation_module/interpolation_module.jl")
using .InterpolationModule
const IM = InterpolationModule

using Plots

ns = [5,10,15]
f1(x) = exp(x)
f2(x) = x^2*sin(x)

for n in ns
    println("n: $n")
    p1 = IM.rysujNnfx(f1, 0.0,1.0,n,100)
    savefig(p1, "func: e^x, n:$n.png")
    gui()
    readline()
    p2 = IM.rysujNnfx(f2, -1.0,1.0, n,100)
    savefig(p2, "func:x^2sinx, n:$n.png")
    gui()
    readline()
end
