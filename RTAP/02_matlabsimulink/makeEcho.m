
%%makeEcho.m

function[out] = makeEcho(in, delay)

X=zeros(1,length(delay));
Z=zeros(length(in), length(delay));
g=zeros(1, length(delay));
for i=(1:length(delay))
    g(i)=10^(-3*delay(i)/100000);
end
for i=(1:length(in))
    out(i) = in(i);
    for j=(1:length(delay))
        if delay(j) < i
           
           X(j)=((in(i-delay(j))+Z(i-delay(j),j))*g(j));
           A=hadamard(length(X))/det(hadamard(length(X)));
           B=X*A;
           for k=(1:length(delay))
              Z(i,k) = B(k); 
           end
           out(i)= out(i)+X(j);
        end
    end
end
    
