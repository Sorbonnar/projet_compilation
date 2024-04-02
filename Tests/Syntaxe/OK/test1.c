// Test programme MiniC
int a = 0, b;
bool c = false;

void main() {
    int i, a = b;
    bool d = false;

    if (c) {
        b;
    }
    for (i = 0; i < 10; i = i + 1) {
        a;
    }
    while (true) {
        a;
    }
    do {
        a;
    }
    while (true);

    a = b * c;
    a = b / c;
    a = b % c;
    a = b + c;
    a = b - c;
    c = b < a;
    c = b > a;
    c = b == a;
    c = b <= a;
    c = b >= a;
    c = b != a;
    c = b && a;
    c = b & a;
    c = b || a;
    c = b | a;
    c = b ^ a;
    c = b << a;
    c = b >> a;
    c = b >>> a;
    c = !d;
    c = ~d;

    print("a: ", a, "\n");
}