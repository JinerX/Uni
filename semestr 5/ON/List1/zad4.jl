# Jędrzej Sajnóg 279701


# for values between 1 and 2 adding epsilon works effectively as nextfloat, this would need to be altered for checking values in a different range

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
