// Expected output:
// True
// a != b
// a < b
// a <= b
//

void main() {
    int a = 100;
    int b = 200;
    bool c = true;
    
    if (c) {
        print("\nTrue");
    }
    else {
        print("\nFalse");
    }
    
    if (a == b) {
        print("\na == b");
    }
    else if (a != b) {
        print("\na != b");
    }

    if (a < b) {
        print("\na < b");
    }
    else if (a > b) {
        print("\na > b");
    }

    if (a <= b) {
        print("\na <= b");
    }
    else if (a >= b) {
        print("\na >= b");
    }
    print("\n");
}
