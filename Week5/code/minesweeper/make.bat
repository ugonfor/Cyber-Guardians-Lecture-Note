g++ -shared -o mydll.dll mydll.cpp -static
g++ -o inject inject.cpp -static