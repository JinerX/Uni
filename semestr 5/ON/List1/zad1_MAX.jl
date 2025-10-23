# Jędrzej Sajnóg 279701


types = [Float16, Float32, Float64]


#=

This returns the exact correct value because in IEEE-754 (and in binary in general) the smallest possible increment is a power of 2 so halving gives an exact value 
therefore we do not need to scan the values between prev and add

=#

function find_epsilon(type)
    a = 1
    x = convert(type, a)
    add = x
    prev = add
    while x + add > 1
        prev = add
        add = add / 2
    end
    return prev
end


function find_max(type)
    x = one(type)
    add = x
    prev = add
    while !isinf(add)
        prev = add
        add = add * 2
    end
    # this way we find the factor of 2, now we're looking for the significand which is the largest float smaller then 2

    return prev * (2 - find_epsilon(type))
end



for type in types
    calc_max = find_max(type)
    true_max = floatmax(type)
    println("Type: $type, calculated max: $calc_max, true_max: $true_max")
end