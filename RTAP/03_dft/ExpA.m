N=64;
x_k1 = zeros (1, N);
x_k2 = zeros (1, N);
for i = 1:1:N
    x_k1(i) = cos((2*pi*10)/N*i);
    x_k2(i) = cos((2*10/N)*i);
end
    