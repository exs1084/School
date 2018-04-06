%% otsu's method
%
% * author: Erikson Sodergren
% end section 

%% Initialization
%
bins = zeros(1, 30);
edges = zeros(1, 30);
for k=1:31
   edges(k)=k+44; 
end
data2=sort(table2array(data));
variances = zeros(1, 30);
bestSpeed=0;
bestVar=999999;

% end section

%% histogram
%
for k=1:height(data)
   dataTemp = data2(k);
   temp=floor(dataTemp);%%fill bins with the number of datapoints in each bin
   bins(temp-44)=bins(temp-44)+1;
end
histogram(data2, edges);%%matlabs built-in histogram function

% end section

%% otsu's method
for k=1:30
   threshold=k+45;
   %find means
   dataL=data2;
   dataR=zeros(1);
   for j=1:height(data)
       if(data2(j)>threshold)
           dataL=data2(1:j-1);
           dataR=data2(j:height(data));
          break 
       end
   end
   countL=size(dataL, 1);
   countR=400-countL;
   fractionL=countL/(countL+countR);
   fractionR=1-fractionL;%%compute fraction of data in each sub-array
   meanL=mean(dataL);
   meanR=mean(dataR);%compute means of subarrays
   varL=var(dataL);
   varR=var(dataR);
   variances(k)=(fractionL*varL)+(fractionR*varR);%%compute mixed variance
   if(variances(k)<=bestVar)
      bestVar=variances(k);
      bestSpeed=threshold;
   end
end

% end section

%% graphing mixed variance
figure()
hold on;
title('Otsus method for speeding cars');
ylabel('mixed variance')
xlabel('Threshold')
plot(45:74, variances);
hold off;
% end section