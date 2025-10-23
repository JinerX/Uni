# Jędrzej Sajnóg 279701


const max_power = 100
const b = Float64(8)

f(x) = Float64(sqrt(x^2 + 1) - 1)
g(x) = Float64(x^2 / (sqrt(x^2 + 1) + 1))


xs = [b^-i for i in 1:1:max_power]

counter = 0
for x in xs
    global counter += 1
    println("$counter & $x & $(f(x)) & $(g(x)) \\\\")
end
