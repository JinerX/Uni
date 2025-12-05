include("./interpolation_module/interpolation_module.jl")
using .InterpolationModule
const IM = InterpolationModule


f(x) = x^2
IM.rysujNnfx(f, -2.0,2.0, 5,50)
