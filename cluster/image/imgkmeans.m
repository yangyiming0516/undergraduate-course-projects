function imgkmeans()
    clear;
    close all;
    clc;
    
load train_img_x_all.mat
load train_img_y_all.mat
num=size(train_x,1);
echo=10;    % try more times to get an average entropy & F
t1 = clock; 
for kk=1:4
    if kk~=2 && kk~=1
        clnum=10*kk-10;   % total clusters
    else if kk==2
        clnum=13;
        else if kk==1
                clnum=10;
            end
        end
    end
ens=zeros(echo,1);
Fs=zeros(echo,1);
for mm=1:echo
[Idx,labels,sumD]=kmeans(train_x,clnum,'dist','cityblock','rep',20,'Start','uniform'); % ,'Start',[train_x(10,:);train_x(82,:);train_x(160,:);train_x(201,:);train_x(340,:);train_x(420,:);train_x(500,:);train_x(560,:);train_x(720,:);train_x(780,:);train_x(950,:);train_x(1050,:);train_x(1150,:)]);
%[Idx,labels]=KMeans(train_x,13);
save MachineLearning2/test_img_x_kmeans.mat Idx
save MachineLearning2/test_img_y_kmeans.mat labels

m=1;
p=zeros(13,clnum);
for i=1:72
    p(1,Idx(i))=p(1,Idx(i))+1;
end
m=m+72;

for i=m:m+78
    p(2,Idx(i))=p(2,Idx(i))+1;
end
m=m+78;

for i=m:m+38
    p(3,Idx(i))=p(3,Idx(i))+1;
end
m=m+38;

for i=m:m+141
    p(4,Idx(i))=p(4,Idx(i))+1;
end
m=m+141;

for i=m:m+78
    p(5,Idx(i))=p(5,Idx(i))+1;
end
m=m+78;

for i=m:m+82
    p(6,Idx(i))=p(6,Idx(i))+1;
end
m=m+82;

for i=m:m+58
    p(7,Idx(i))=p(7,Idx(i))+1;
end
m=m+58;

for i=m:m+158
    p(8,Idx(i))=p(8,Idx(i))+1;
end
m=m+158;

for i=m:m+63
    p(9,Idx(i))=p(9,Idx(i))+1;
end
m=m+63;

for i=m:m+170
    p(10,Idx(i))=p(10,Idx(i))+1;
end
m=m+170;

for i=m:m+97
    p(11,Idx(i))=p(11,Idx(i))+1;
end
m=m+97;

for i=m:m+107
    p(12,Idx(i))=p(12,Idx(i))+1;
end
m=m+107;

for i=m:m+73
    p(13,Idx(i))=p(13,Idx(i))+1;
end
m=m+73;

save MachineLearning2/Pij_kmeans.mat p
[entropy,F]=Evaluation(clnum);

ens(mm)=entropy;
Fs(mm)=F;
end

x=1:1:10;
figure(1),plot(x,ens(x),'-','color',[0.5,kk*0.25,0]),hold on
xlabel('t')
ylabel('entropy')
title('Cluster Analysis')
axis([1,10,0,2]);

figure(2),plot(x,Fs(x),'-','color',[0.5,kk*0.25,0]),hold on
xlabel('t')
ylabel('F')
title('Cluster Analysis')
axis([1,10,0,1]);

Entropy=mean(ens(:,1))
F=mean(Fs(:,1))
end


etime(clock,t1)

end



function [entropy,Fval]=Evaluation(clnum)
%clear;
%close all;
%clc;
load MachineLearning2/Pij_kmeans.mat
load MachineLearning2/test_img_x_kmeans.mat

cluster=zeros(clnum,1);
for j=1:clnum
    cluster(j)=sum(p(:,j));
end


entropy=0;
en=zeros(clnum,1);
for j=1:clnum
     for i=1:13
         if cluster(j)~=0
         pij=p(i,j)/cluster(j);
         if pij~=0
             en(j)=en(j)+pij*log(pij);
         end
         end
     end
     en(j)=-en(j);
    entropy=entropy+en(j)*cluster(j)/1227;
end

r=zeros(13,1);
f=zeros(13,clnum);
for i=1:13
    r(i)=sum(p(i,:));
end

for i=1:13
    for j=1:clnum
        if cluster(j)~=0
        rij=p(i,j)/r(i);
        pij=p(i,j)/cluster(j);
        f(i,j)=2*rij*pij/(rij+pij);
        end
    end
end
Fval=0;
for i=1:13
    Fval=Fval+r(i)/1227*max(f(i,:));
end

%clnum
%entropy
%Fval

end






%N是数据一共分多少类
%data是输入的不带分类标号的数据
%u是每一类的中心
%re是返回的带分类标号的数据
function [u re]=KMeans(data,N)   
    [m n]=size(data);   %m是数据个数，n是数据维数
    ma=zeros(n);        %每一维最大的数
    mi=zeros(n);        %每一维最小的数
    u=zeros(N,n);       %随机初始化，最终迭代到每一类的中心位置
    for i=1:n
       ma(i)=max(data(:,i));    %每一维最大的数
       mi(i)=min(data(:,i));    %每一维最小的数
       for j=1:N
            u(j,i)=ma(i)+(mi(i)-ma(i))*rand();  %随机初始化，不过还是在每一维[min max]中初始化好些
       end      
    end
   
    while 1
        pre_u=u;            %上一次求得的中心位置
        for i=1:N
            tmp{i}=[];      % 公式一中的x(i)-uj,为公式一实现做准备
            for j=1:m
                tmp{i}=[tmp{i};data(j,:)-u(i,:)];
            end
        end
        
        quan=zeros(m,N);
        for i=1:m        %公式一的实现
            c=[];
            for j=1:N
                c=[c norm(tmp{j}(i,:))];
            end
            [junk index]=min(c);
            quan(i,index)=norm(tmp{index}(i,:));           
        end
        
        for i=1:N            %公式二的实现
           for j=1:n
                u(i,j)=sum(quan(:,i).*data(:,j))/sum(quan(:,i));
           end           
        end
        
        if norm(pre_u-u)<0.1  %不断迭代直到位置不再变化
            break;
        end
    end
    
    re=[];
    for i=1:m
        tmp=[];
        for j=1:N
            tmp=[tmp norm(data(i,:)-u(j,:))];
        end
        [junk index]=min(tmp);
        re=[re;data(i,:) index];
    end
    
end

