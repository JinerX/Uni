# First attempt - naive approach (maybe it's close to 1)

function find_smallest_approx()
    x = one(Float64)
    while x < Float64(2)
        if x * (1/x) != 1
            return x
        end
        x = x + eps(Float64)
    end
end

a = find_smallest_approx()
solution = a * (1/a)
println("smallest a such that a* (1/a) /= 1 is: $a, instead it equals $solution")
