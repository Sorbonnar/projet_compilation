int int1 = 10;

void main() {
    int a = 5;
    int b = int1;
    int i;
    bool c = true;
    
    int sum = a + b;
    print("Sum:", sum);
    
    if (c) {
        print("true");
    } else {
        print("false");
    }
    
    while (a < 8) {
        print("a:", a);
        a = a + 1;
    }

    for (i = 0; i < 5; i = i + 1) {
        print("i:", i);
    }

    do {
        b = b - 1;
        print("b:", b);
    } while (b > 5);
}
