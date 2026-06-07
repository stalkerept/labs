import random

def generate_social_graph(num_vertices, extra_edge_prob=0.02, seed=42):
    random.seed(seed)
    edges = []
    for i in range(num_vertices - 1):
        person1 = f"Person{i}"
        person2 = f"Person{i+1}"
        weight = random.randint(-10, 10)
        edges.append(f"{person1} {person2} {weight}")

    for i in range(num_vertices):
        for j in range(i+1, num_vertices):
            if random.random() < extra_edge_prob:
                weight = random.randint(-10, 10)
                edges.append(f"Person{i} Person{j} {weight}")
    random.shuffle(edges)
    with open("file.txt", "w", encoding="utf-8") as file:
        file.write("\n".join(edges) + "\n")

if __name__ == "__main__":
    NUM_PEOPLE = 300
    EXTRA_EDGE_PROB = 0.02

    generate_social_graph(NUM_PEOPLE, EXTRA_EDGE_PROB)