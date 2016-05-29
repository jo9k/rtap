N=32000;
audio = zeros(1,N);
for i = 1:1:N
   audio(1,i) = ausgabe(1,1,i); 
end
sound (audio)