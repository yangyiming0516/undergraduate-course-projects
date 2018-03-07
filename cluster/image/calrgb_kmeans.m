function calrgb_kmeans()
    clear;
    close all;
    clc;
    topdir='/Users/SK/Documents/MATLAB/MachineLearning2/img';
    file=dir(topdir);
    num=5;         % num of hists
    traini=1;       % order of img
    testi=1;
    train_x=zeros(1227,num*3);
    train_y=zeros(1227,1);
    for i=1:length(file)-2;
        str=dir([topdir,'/',file(i+2).name,'/*.jpg']);
        for j=1:length(str);
            im=imread(str(j).name);
            [h,w]=size(im);
            s=w*h/3;
            imgr=im(:,:,1);
            imgg=im(:,:,2);
            imgb=im(:,:,3);
            hist_r=imhist(imgr,num); %计算直方图
            hist_g=imhist(imgg,num);
            hist_b=imhist(imgb,num);
            train_x(traini,1:num)=double(reshape(hist_r,1,num))/s;
            train_x(traini,num+1:num*2)=double(reshape(hist_g,1,num))/s;
            train_x(traini,num*2+1:num*3)=double(reshape(hist_b,1,num))/s;
            train_y(traini)=i;
            traini=traini+1;
            % bar(hist_im);%画直方图
        end
    end
    traini=traini-1;
    testi=testi-1;
    train_x=train_x(1:traini,:);
    train_y=train_y(1:traini,:);
    save MachineLearning2/train_img_x_all train_x;
    save MachineLearning2/train_img_y_all train_y;
    %imgkmeans();
end