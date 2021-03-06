# undergraduate-course-projects
course projects during my undergraduate study. Only part of them, not even representative, but already all I can find after so many years.

## Tetris
A special Tetris game with triangles and trapezoids. save/load is supported.  
The game is written in C++ and WIN32 API.  
![Tetris Sample](https://raw.githubusercontent.com/yangyiming0516/undergraduate-course-projects/master/Tetris/sample.png)

## Visualization
Some visualization implemented with D3.js. There are different designs for different datasets.
### hotel
[demo link](http://vis.pku.edu.cn/course/Visualization_2014F/hotel/yangyiming/)
#### dataset
Guests data in a hotel during each month, including sex, booking type, age, source, etc. 
#### design
pie and histogram charts are used to display different attributes.  
the data of each month can be selected.  
![pie&histogram charts](https://raw.githubusercontent.com/yangyiming0516/undergraduate-course-projects/master/Visualization/hotel/sample1.png)
the charts are interactive so different parts of the data can be added to the line chart to show the trends of each month.  
![trends](https://raw.githubusercontent.com/yangyiming0516/undergraduate-course-projects/master/Visualization/hotel/sample2.png)

### wine
[demo link](http://vis.pku.edu.cn/course/Visualization_2014F/wine/yangyiming/)
#### dataset
High-dimensional data of multiple attributes of different of wine
#### design
![sample](https://raw.githubusercontent.com/yangyiming0516/undergraduate-course-projects/master/Visualization/wine/sample.png)  
Parallel axis chart is used, the axis can be dragged.  
A scatter chart is used to observe the relation between two specific attributes, the attributes can be chosen.  
The amount of different kinds of wine selected are displayed by histograms.  
All charts are interactive, the data chosen in one chart are also selected correspondingly in other charts.

### weibo
[demo link](http://vis.pku.edu.cn/course/Visualization_2014F/final_project/group5/)
#### dataset
SNS data from weibo, containing gps and post content.
#### degisn
![sample](https://raw.githubusercontent.com/yangyiming0516/undergraduate-course-projects/master/Visualization/weibo/sample.png)  
A heatmap shows the distribution of the messages posted. Areas can be chosen.
![heatmap](https://raw.githubusercontent.com/yangyiming0516/undergraduate-course-projects/master/Visualization/weibo/map.png)  
A trend chart shows the amount of weibo posted one different time. Time scopes can be chosen.
![time](https://raw.githubusercontent.com/yangyiming0516/undergraduate-course-projects/master/Visualization/weibo/time.png)  
Histograms show the amount of weibo selected by area or time. There are also functions such as animation or zooming of the time axis.  
![animation](https://raw.githubusercontent.com/yangyiming0516/undergraduate-course-projects/master/Visualization/weibo/Animation.gif) 

### VAST 2008 Challenge 1
[demo link](http://vis.pku.edu.cn/course/Visualization_2014F/vast2008/group4/c1/)
#### dataset
VAST 2008 Mini Challenge-1, Wiki Editors
see details on VAST challenge 08 [official site](https://www.cs.umd.edu/hcil/VASTchallenge08/tasks.html)
#### design
![sample1](https://raw.githubusercontent.com/yangyiming0516/undergraduate-course-projects/master/Visualization/vast/sample1.png)  
![sample2](https://raw.githubusercontent.com/yangyiming0516/undergraduate-course-projects/master/Visualization/vast/sample2.png) 

## Natural Language Processing
some basic nlp problems such as Ngrams and Backoff Models, word segmentation(Chinese)  
The codes are written in C++
### ngram, smoothing, backoff
algorithm to judge which one of two sentences is a real and correct sentence.  
Count the frequencies of unigram and bigram. calculate the probability of a sentence based on the frequencies.  
smooth the counts with add-del or good-turing smoothing.  
tried add-del+backoff and good-turing + katz backoff  
see details in readme
### word segmentation(Chinese)
simple implementation of normal perceptron (F-Score:0.913) and structured perceptron (F-Score:0.930)  
see details in readme

## Cluster
K-means algorithm to cluster text and image data  
the text cluster is written in C++, tf / tfidf are used as feature vectors  
the image cluster is written in matlab, hsd / rgb features are used.

## Robot navigation
naive and A-star algorithms to navigate a robot which has distance sensors to several directions. The robot goes to a set coordinate(the starting point is 0,0)  
the code was run in a virtual environment to test at first, then ran on a robot provided by the course in a real room.  
The virtual environment was not kept but there are some demo videos recorded.