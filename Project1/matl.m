clearvars;
close all;
clc;

fil = fopen('data.pcd','r');
line = fgetl(fil);
G = regexp(line,' ','split');
WIDTH = 0;
HEIGHT = 0;
WIDTH = str2double(G(2));
HEIGHT = str2double(G(4));
A=cell(HEIGHT,WIDTH+1);

x=zeros(HEIGHT,WIDTH);
y=zeros(HEIGHT,WIDTH);
z=zeros(HEIGHT,WIDTH);

for i=1:HEIGHT
    line=fgetl(fil);
    k = regexp(line,';','split');
    A(i,:)=k;
    for j=1:WIDTH-1
        m = regexp(k(1,j),' ','split');
        x(i,j)=str2double(m{1,1}(1));
        y(i,j)=str2double(m{1,1}(2));
        temp=str2double(m{1,1}(3));
        if(temp==-2000)
            z(i,j)=NaN;
        else
            z(i,j)=temp;
        end
    end
end
fclose(fil);
surf(z,x,y);

