include("tmp.jl")
using .TestModule
using Test

const TM = TestModule

@test TM.add(1,2) == 3
@test TM.mult(2,3) == 6
@test TM.mult(2,0) == 0
@test TM.add(9,7) == 16