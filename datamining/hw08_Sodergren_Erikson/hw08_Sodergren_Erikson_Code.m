%% PCA+k-means
% * author: Erikson Sodergren
% end section 

%% Initialization
%
% If there are numbers that you need to define up front, or that you want
% easily accessible, define them in this section.
filename = fullfile('shopData.csv');
T = readtable(filename);
dataArray = table2array(T);
dataArray=dataArray(:,2:13);
avg=mean(dataArray);
dataArray = dataArray-avg;
% end section

%% Covariance

C=cov(dataArray);%%?????
% end section

%% find eigenvalues
%
[vectors, projection, values]=pca(C);
numVals = size(values);
relVals = values/sum(values);
cummVals = zeros(numVals);
for i = 1:numVals
   cummVals(i:numVals) =  cummVals(i:numVals)+relVals(i);
end
cummVals(2:numVals+1) = cummVals(1:numVals);
cummVals(1)=0;
figure()
hold on;
title('cummulative importance of eigenvectors');
ylabel('cummulative variance captured')
xlabel('number of vectors')
plot( 0:11, cummVals, 'bo');
plot( 0:11, cummVals);
hold off;

fprintf("the two most useful eigenvectors are:");
projectMat = vectors(:,1:2)

%removing petfood now

% end section
%% plot on projection
projPlot = dataArray * projectMat;

figure()
hold on;
title('projected onto 2 dimensions');
ylabel('amount of priciple component 2')
xlabel('amount of priciple component 1')
scatter( projPlot(:,1), projPlot(:,2));
hold off;
% end section

%% kmeans
[idx, centroids] = kmeans(projPlot, 3);
colors = zeros(3,3);
colors(1,:)=[1,0,0];
colors(2,:)=[1,0,1];
colors(3,:)=[0,0,0];
hold on;
scatter(centroids(:,1), centroids(:,2), 150, colors, 'd', 'filled');
hold off;

fprintf("the 3 cluster centroids are:");
centroids

% end section

%% convert centers to full dimensions
revProject = projectMat.';
clusters = centroids * revProject

% end section