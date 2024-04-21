// Expected output:
// Count: 5
// Count: 4
// Count: 3
// Count: 2
// Count: 2
//

int count = 5;

void main() {
    while (count > 1) {
        print("Count: ", count, "\n");
        count = count - 1;
    }

    count = count + 1;

    print("Count: ", count, "\n");
}
