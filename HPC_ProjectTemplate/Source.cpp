#include <iostream>
#include <math.h>
#include <stdlib.h>
#include<string.h>
#include<msclr\marshal_cppstd.h>
#include <ctime>// include this header 
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
using namespace std;
using namespace msclr::interop;

int* inputImage(int* w, int* h, System::String^ imagePath) //put the size of image in w & h
{
	int* input;


	int OriginalImageWidth, OriginalImageHeight;

	//*********************************************************Read Image and save it to local arrayss*************************	
	//Read Image and save it to local arrayss

	System::Drawing::Bitmap BM(imagePath);

	OriginalImageWidth = BM.Width;
	OriginalImageHeight = BM.Height;
	*w = BM.Width;
	*h = BM.Height;
	int *Red = new int[BM.Height * BM.Width];
	int *Green = new int[BM.Height * BM.Width];
	int *Blue = new int[BM.Height * BM.Width];
	input = new int[BM.Height*BM.Width];
	for (int i = 0; i < BM.Height; i++)
	{
		for (int j = 0; j < BM.Width; j++)
		{
			System::Drawing::Color c = BM.GetPixel(j, i);

			Red[i * BM.Width + j] = c.R;
			Blue[i * BM.Width + j] = c.B;
			Green[i * BM.Width + j] = c.G;

			input[i*BM.Width + j] = ((c.R + c.B + c.G) / 3); //gray scale value equals the average of RGB values

		}

	}
	return input;
}


void createImage(int* image, int width, int height, int index)
{
	System::Drawing::Bitmap MyNewImage(width, height);


	for (int i = 0; i < MyNewImage.Height; i++)
	{
		for (int j = 0; j < MyNewImage.Width; j++)
		{
			//i * OriginalImageWidth + j
			if (image[i*width + j] < 0)
			{
				image[i*width + j] = 0;
			}
			if (image[i*width + j] > 255)
			{
				image[i*width + j] = 255;
			}
			System::Drawing::Color c = System::Drawing::Color::FromArgb(image[i*MyNewImage.Width + j], image[i*MyNewImage.Width + j], image[i*MyNewImage.Width + j]);
			MyNewImage.SetPixel(j, i, c);
		}
	}
	MyNewImage.Save("..//Data//Output//outputRes" + index + ".png");
	cout << "result Image Saved " << index << endl;
}


int main()
{
	int ImageWidth = 320, ImageHeight = 240;
	int pixels = ImageHeight * ImageWidth;
	int start_s, stop_s, TotalTime = 0;


	
	//int** allImages = new int* [495];
	int* allImages = new int[ImageWidth * ImageHeight * 496];
		for (int i = 0; i < 496; i++) {
			System::String^ imagePath;
			std::string img;

			string imgNumber;
			std::string str = to_string(i + 1);
			int n = 6;
			int precision = n - str.size();
			std::string s = std::string(precision, '0').append(str);

			img = "..//Data//BackGround//in";
			img = img.append(s);
			img = img.append(".jpg");

			imagePath = marshal_as<System::String^>(img);
			int* temp =  inputImage(&ImageWidth, &ImageHeight, imagePath);

			for (int j = 0; j < pixels; j++) {

				allImages[(i * pixels ) + j] = temp[j];
			}

			//createImage(allImages[i], ImageWidth, ImageHeight, i);
		}

		/*int* arr = new int[pixels];
		int count = 0;
		for (int i = 0; i < pixels ; i++)
		{
			
				arr[i] = allImages[(0*pixels+1)+i];
			
		}
		createImage(arr, ImageWidth, ImageHeight, 0);*/
	
		start_s = clock();



		int* mean = new int[ImageWidth * ImageHeight];
		
		for (int i = 0; i < pixels; i++) {
			int localSum = 0;
			for (int j = 0; j < 496; j++) {

				localSum += allImages[(j * pixels )+i];
			}
			mean[i] = localSum / 496;
		}

		createImage(mean, ImageWidth, ImageHeight, 0);

		System::String^ path;

		path = marshal_as<System::String^>("..//Data//BackGround//in000256.jpg");
		int* image_musked = inputImage(&ImageWidth, &ImageHeight, path);

		for (int i = 0; i < pixels; i++)
		{
			if (abs(mean[i] - image_musked[i]) > 60)
			{
				
			}
			else
			{
				image_musked[i] = 0;
			}
		}
		
		createImage(image_musked, ImageWidth, ImageHeight, 1);


		stop_s = clock();
		TotalTime += (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000;
		cout << "time: " << TotalTime << endl;















	/*System::String^ imagePath;
	std::string img;
	img = "..//Data//Input//test.png";

	imagePath = marshal_as<System::String^>(img);
	int* imageData = inputImage(&ImageWidth, &ImageHeight, imagePath);


	start_s = clock();
	createImage(imageData, ImageWidth, ImageHeight, 0);
	stop_s = clock();
	TotalTime += (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000;
	cout << "time: " << TotalTime << endl;

	free(imageData);*/
		system("pause");
	return 0;

}



