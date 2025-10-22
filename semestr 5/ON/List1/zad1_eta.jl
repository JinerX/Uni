types = [Float16, Float32, Float64]

function find_eta(type)
    a = zero(type)
    x = convert(type, a)
    add = one(type)
    prev = add
    while x + add > 0
        prev = add
        add = add / 2
    end
    return prev
end


for type in types
    calc_eta = find_eta(type)
    true_eta = nextfloat(zero(type))
    println("Type: $type, calculated eta: $calc_eta, true_eta: $true_eta")
end