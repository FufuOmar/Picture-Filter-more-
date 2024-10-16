#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avg = round((image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / 3.0);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    int newLoc;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width/2 ; j++)
        {
            newLoc = width-j-1;
            temp = image[i][newLoc];
            image[i][newLoc] = image[i][j];
            image[i][j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurImage[height][width];
    int newHeight;
    int newWidth;


    for(int i = 0; i<height; i++)
    {
        for(int j = 0; j<width; j++) // Selects the pixel to blur
        {
            int pixelCount = 0;
            int holdRed = 0;
            int holdGreen = 0;
            int holdBlue = 0;
                for(int offsetH = -1; offsetH < 2; offsetH++) //Gets offset for height
                {
                    newHeight = i + offsetH;
                    for(int offsetW = -1; offsetW < 2; offsetW++) //Gets offset for width
                        {
                            newWidth = j + offsetW;
                            if(newHeight >= 0 && newHeight < height && newWidth >= 0 && newWidth < width)//Checks if its still in the boundarys of the image
                            {
                                    holdRed += image[newHeight][newWidth].rgbtRed;
                                    holdBlue += image[newHeight][newWidth].rgbtBlue;
                                    holdGreen += image[newHeight][newWidth].rgbtGreen;
                                    pixelCount++;
                            }
                        }

                }
            holdRed = round((float)holdRed/pixelCount);
            holdGreen = round((float)holdGreen/pixelCount);
            holdBlue = round((float)holdBlue/pixelCount);

            blurImage[i][j].rgbtRed = holdRed;
            blurImage[i][j].rgbtGreen = holdGreen;
            blurImage[i][j].rgbtBlue = holdBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurImage[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Declare a variable to store copy the copy of the image
    RGBTRIPLE copy[height][width];
    // Gx and Gy constants
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Creates a copy of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Loops through each row in the image
    for (int i = 0; i < height; i++)
    {
        // Loops through each column in the iterated row
        for (int j = 0; j < width; j++)
        {
            // Initialize gx & gy values for each channel
            double gx_red = 0, gx_green = 0, gx_blue = 0;
            double gy_red = 0, gy_green = 0, gy_blue = 0;

            // Iteration thru all the pixels in a 3x3 grid
            for (int row = -1; row < 2; row++)
            {
                for (int col = -1; col < 2; col++)
                {
                    // Checks for bounds
                    if (row + i < 0 || row + i > height - 1 || col + j < 0 || col + j > width - 1)
                    {
                        continue;
                    }

                    gx_red += copy[i + row][j + col].rgbtRed * gx[row + 1][col + 1];
                    gx_green += copy[i + row][j + col].rgbtGreen * gx[row + 1][col + 1];
                    gx_blue += copy[i + row][j + col].rgbtBlue * gx[row + 1][col + 1];

                    gy_red += copy[i + row][j + col].rgbtRed * gy[row + 1][col + 1];
                    gy_green += copy[i + row][j + col].rgbtGreen * gy[row + 1][col + 1];
                    gy_blue += copy[i + row][j + col].rgbtBlue * gy[row + 1][col + 1];
                }
            }
            // square root of gx^2 + gy^2

            double sobel_red = sqrt(((gx_red * gx_red) + (gy_red * gy_red)));
            double sobel_green = sqrt(((gx_green * gx_green) + (gy_green * gy_green)));
            double sobel_blue = sqrt(((gx_blue * gx_blue) + (gy_blue * gy_blue)));

            if (sobel_red > 255)
            {
                sobel_red = 255;
            }
            if (sobel_green > 255)
            {
                sobel_green = 255;
            }
            if (sobel_blue > 255)
            {
                sobel_blue = 255;
            }

            image[i][j].rgbtRed = (int) round(sobel_red);
            image[i][j].rgbtGreen = (int) round(sobel_green);
            image[i][j].rgbtBlue = (int) round(sobel_blue);
        }
    }

    return;
}