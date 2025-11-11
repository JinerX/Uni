using HiGHS
using JuMP


# Linia 1: Liczba dystryktów
# Linia 2: Liczba zmian
#
# Następne linie (tyle, ile jest zmian): Minimalna liczba radiowozów na zmianę
#
# Następne linie (tyle, ile jest dystryktów): Minimalna liczba radiowozów na dystrykt
#
# Następne linie (blok 'dystrykty' x 'zmiany'): Macierz minimalnej liczby radiowozów dla dystryktu i zmiany
#   (każdy wiersz to jeden dystrykt, wartości oddzielone spacjami)
#
# Następne linie (blok 'dystrykty' x 'zmiany'): Macierz maksymalnej liczby radiowozów dla dystryktu i zmiany
#   (taka sama struktura jak wyżej)
function get_input_values(filename)
    open(filename, "r") do file
        # Wczytanie wymiarów
        districts = parse(Int64, readline(file))
        shifts = parse(Int64, readline(file))

        # Inicjalizacja wektorów i macierzy
        min_per_shift = []
        min_per_district = []
        min_cars_district_shift = zeros(Int64, districts, shifts)
        max_cars_district_shift = zeros(Int64, districts, shifts)

        # Wczytanie wektora min_per_shift
        for _ in 1:shifts
            push!(min_per_shift, parse(Int64, readline(file)))
        end

        # Wczytanie wektora min_per_district
        for _ in 1:districts
            push!(min_per_district, parse(Int64, readline(file)))
        end

        # Wczytanie macierzy min_cars_district_shift
        for i in 1:districts
            line = readline(file)
            parts = split(line)
            for j in 1:shifts
                min_cars_district_shift[i, j] = parse(Int64, parts[j])
            end
        end

        # Wczytanie macierzy max_cars_district_shift
        for i in 1:districts
            line = readline(file)
            parts = split(line)
            for j in 1:shifts
                max_cars_district_shift[i, j] = parse(Int64, parts[j])
            end
        end

        # Zwrócenie wszystkich wczytanych danych
        return districts, shifts, min_per_shift, min_per_district, min_cars_district_shift, max_cars_district_shift
    end
end


districts = 3
shifts = 3
min_per_shift = [10,20,18]
min_per_district = [10,14,13]

min_cars_district_shift = [2 4 3;
                           3 6 5;
                           5 7 6]

max_cars_district_shift = [3 7 5;
                           5 7 10;
                           8 12 10]

districts, shifts, min_per_shift, min_per_district, min_cars_district_shift, max_cars_district_shift = get_input_values(ARGS[1])

model = Model(HiGHS.Optimizer)

@variable(model, x[1:districts, 1:shifts])
set_lower_bound.(x, min_cars_district_shift)
set_upper_bound.(x, max_cars_district_shift)

for i in 1:districts
    @constraint(model, sum(x[i,:]) >= min_per_district[i])
end

for i in 1:shifts
    @constraint(model, sum(x[:,i]) >= min_per_shift[i])
end

@objective(model, Min, sum(x))

optimize!(model)
println("termination status: ", termination_status(model))
println("objective cars total: ", objective_value(model))
for i in 1:districts
    for j in 1:shifts
        println("number of police carrs assigned to district $i adn shift $j: $(value(x[i,j]))")
    end
end
