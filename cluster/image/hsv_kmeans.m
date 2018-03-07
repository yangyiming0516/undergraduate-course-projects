function hsv_kmeans()
    clear;
    close all;
    clc;
    topdir='/Users/SK/Documents/MATLAB/MachineLearning2/img';
    file=dir(topdir);
    num=10;         % num of hists
    traini=1;       % order of img
    testi=1;
    train_x=zeros(1227,num*3);
    train_y=zeros(1227,1);
    for i=1:length(file)-2;
        str=dir([topdir,'/',file(i+2).name,'/*.jpg']);
        for j=1:length(str);
            im=imread(str(j).name);
            imgh=gethsv(im,num);
            % [image, descrips, locs]=sift(rgb2gray(im));
            % showkeys(image, locs); 
            % pause;
            % train_x(traini,:)=double(imgh);
            train_x(traini,:)=double(reshape(imgh,1,num*3));
            train_y(traini)=i;
            traini=traini+1;
            % bar(hist_im);%»­Ö±·½Í¼
        end
    end
    traini=traini-1;
    testi=testi-1;
    train_x=train_x(1:traini,:);
    train_y=train_y(1:traini,:);
    save MachineLearning2/train_img_x_all train_x;
    save MachineLearning2/train_img_y_all train_y;
    
end

function histh=gethsv(p,num)
%figure(1),imshow(p);
[h,s,v] = rgb2hsv(p);
%figure(2),imhist(h,num);
%figure(3),imhist(s,num);
%figure(4),imhist(v,num);
%histh=imhist(h,num);
histh=[imhist(h,num) imhist(s,num) imhist(v,num)];

end