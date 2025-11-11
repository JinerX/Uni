using HiGHS
using JuMP


# Linia 1: Wysokość (m), Szerokość (n), Zasięg kamery (k) - oddzielone spacjami
# Linia 2: Liczba kontenerów
# Kolejne linie: Współrzędne każdego kontenera (wiersz kolumna) - oddzielone spacjami
function get_input_values(filename)
    container_locations = []

    open(filename, "r") do file
        line1 = readline(file)
        parts1 = split(line1)
        m = parse(Int64, parts1[1])
        n = parse(Int64, parts1[2])
        k = parse(Int64, parts1[3])

        n_containers = parse(Int64, readline(file))

        for _ in 1:n_containers
            line = readline(file)
            parts = split(line)
            row = parse(Int64, parts[1])
            col = parse(Int64, parts[2])
            
            push!(container_locations, (row, col))
        end
        
        return m, n, k, container_locations
    end
end

m = 10
n = 15
k = 1

function print_grid(grid, cameras)
    for row in 1:size(grid)[1]
        for col in 1:size(grid)[2]
            if grid[row,col] == 1
                print("C ")
            elseif cameras[row,col] ≈ 1
                print("M ")
            else
                print("_ ")
            end
        end
        println()
    end
end




grid = zeros(m,n)

# contener_locations = [
#     (5,10),
#     (1,4),
#     (3,4),
#     (7,12),
#     (8,12),
#     (9,3),
#     (6,2)
# ]

m, n, k, contener_locations = get_input_values(ARGS[1])

for loc in contener_locations
    grid[loc[1],loc[2]] = 1
end

model = Model(HiGHS.Optimizer)
@variable(model, x[1:m,1:n], Bin) # camera or no camera
@objective(model, Min, sum(x))
for loc in contener_locations
    row = loc[1]
    col = loc[2]
    @constraint(model, sum(x[row:min(row+k,m),col]) + sum(x[row,col:min(col+k,n)]) + sum(x[max(1,row-k):row,col]) + sum(x[row, max(1,col-k):col])>= 1)
    @constraint(model, x[row,col] == 0)
end

optimize!(model)
println("termination status: ", termination_status(model))

cameras = value(x)
print_grid(grid, cameras)

