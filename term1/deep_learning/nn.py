from miniflow import *

inputs, weights, bias = Input(), Input(), Input()

f = Linear(inouts, weights, bias)

feed_dict = {
    inputs: [6, 14, 3],
    weights: [0.5, 0.25, 1.4],
    bias: 2
}

graph = topological_sort(feed_dict)
output = forward_pass(f, sorted_nodes)

# NOTE: because topological_sort sets the values for the `Input` nodes we could also access
# the value for x with x.value (same goes for y).
# print("{} + {} = {} (according to miniflow)".format(feed_dict[x], feed_dict[y], output))

print(output)