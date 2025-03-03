#pip install redis numpy
#pip show redis
import redis
import numpy as np

# Connect to Redis
r = redis.Redis(host='localhost', port=6379, decode_responses=False)

# Function to calculate cosine similarity
def cosine_similarity(v1, v2):
    return np.dot(v1, v2) / (np.linalg.norm(v1) * np.linalg.norm(v2))

class Charm:
    def __init__(self, name: str, id: int):
        self.name = name
        self.id = id

    def get_bytes(self):
        encoded = self.name.encode('utf-8')
        array = bytearray(encoded)
        array.extend(self.id.to_bytes())

        return np.array(array, dtype=np.float32)


# Clear previous data
r.flushall()

# Charm name : id
charms = {
    "Charm one": 20,
    "Charm Two": 15,
    "Charm Three": 25,
}

# Add charms
for charm_name, charm_id in charms.items():
    charm = Charm(charm_name, charm_id)
    vector = charm.get_bytes().tobytes()
    print(vector)

    r.set(f"charm:{charm_name}", vector)
    print(f"Added charm: {charm_name} with vector: {vector}")

# Query vector
query_vector = np.array([10, 20, 40, 30, 50, 55, 65, 22, 23, 10], dtype=np.float32)
print(f"\nQuery Vector: {query_vector}")

def sanitize(arr):
    x = []
    if len(arr) > 10:
        for val in range(0, 10):
            x.append(arr[val].item())
        return x
    else:
        return arr


# Manual similarity search
results = []
for charm_name, charm_id in charms.items():
    charm = Charm(charm_name, charm_id)
    vector = sanitize(charm.get_bytes())
    print(vector)

    similarity = cosine_similarity(query_vector, vector)
    results.append((charm_name, similarity))

# Sort and display results
results.sort(key=lambda x: x[1], reverse=True)
print("\nSimilarity Search Results:")
for charm_name, similarity in results:
    print(f"Charm: {charm_name}, Similarity: {similarity:.4f}")