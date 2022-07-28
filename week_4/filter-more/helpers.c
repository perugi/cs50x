#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // The average value of all the three RGB channels
    BYTE avgVal;

    // Loop through each row and column.
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            // Calculate the average value of the three RGB channels.
            avgVal = round((image[row][col].rgbtRed +
                            image[row][col].rgbtGreen +
                            image[row][col].rgbtBlue) /
                           3.0);

            // To convert to grayscale, assign each of the channels the average value.
            image[row][col].rgbtRed = avgVal;
            image[row][col].rgbtGreen = avgVal;
            image[row][col].rgbtBlue = avgVal;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // The tmp variable will store the temporary value during the swapping of the two pixels.
    RGBTRIPLE tmp;

    // Loop through each row and column.
    for (int row = 0; row < height; row++)
    {
        for (int col = 0, middle = round(width / 2.0 - 1); col <= middle; col++)
        {
            // For each row, go through the first half of the pixels and swap them
            // with an appropriate pixedl from the second half.
            tmp = image[row][col];
            image[row][col] = image[row][width - col - 1];
            image[row][width - col - 1] = tmp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // A temporary image is needed to store the calculated values.
    RGBTRIPLE blurredImage[height][width];

    // Loop through each row and column.
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            // These variables will store the sums for each of the channels, out of which
            // the averages values will be calculated.
            int sumPixelRed = 0;
            int sumPixelGreen = 0;
            int sumPixelBlue = 0;
            int pixelCount = 0;

            // For each pixel, create an average out of all the surrounding pixels.
            for (int row_delta = -1; row_delta <= 1; row_delta++)
            {
                for (int col_delta = -1; col_delta <= 1; col_delta++)
                {
                    // Make sure to properly handle the border pixels - if we would be
                    // out of image bounds, do not try to add to the average pixel.
                    if (row + row_delta >= 0 &&
                        row + row_delta <= height - 1 &&
                        col + col_delta >= 0 &&
                        col + col_delta <= width - 1)
                    {
                        sumPixelRed += image[row + row_delta][col + col_delta].rgbtRed;
                        sumPixelGreen += image[row + row_delta][col + col_delta].rgbtGreen;
                        sumPixelBlue += image[row + row_delta][col + col_delta].rgbtBlue;
                        pixelCount += 1;
                    }
                }
            }

            // Calculate the actual averages from the sums and the number of pixels
            // that were added to the sums.
            blurredImage[row][col].rgbtRed = round(sumPixelRed / (float)pixelCount);
            blurredImage[row][col].rgbtGreen = round(sumPixelGreen / (float)pixelCount);
            blurredImage[row][col].rgbtBlue = round(sumPixelBlue / (float)pixelCount);
        }
    }

    // Copy the blurred image into the original one. We could not perform this in-situ,
    // as changing any of the pixels on the original image would corrupt the average
    // calculation for the next pixels.
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            image[row][col] = blurredImage[row][col];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create an image with a border of a single line of black pixels.
    RGBTRIPLE borderedImage[height + 2][width + 2];

    // A temporary image is needed to store the calculated values.
    RGBTRIPLE resultImage[height][width];

    // The x kernel for the Sobel operator.
    int kernelX[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    // The Y kernel for the Sobel operator.
    int kernelY[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int row = 0; row < height + 2; row++)
    {
        for (int col = 0; col < width + 2; col++)
        {
            if (row == 0 || row == height + 1 || col == 0 || col == width + 1)
            {
                borderedImage[row][col].rgbtRed = 0;
                borderedImage[row][col].rgbtGreen = 0;
                borderedImage[row][col].rgbtBlue = 0;
            }
            else
            {
                borderedImage[row][col] = image[row - 1][col - 1];
            }
        }
    }

    // Loop through each row and column.
    for (int row = 1; row < height + 1; row++)
    {
        for (int col = 1; col < width + 1; col++)
        {
            int xSumPixelRed = 0;
            int xSumPixelGreen = 0;
            int xSumPixelBlue = 0;

            int ySumPixelRed = 0;
            int ySumPixelGreen = 0;
            int ySumPixelBlue = 0;

            for (int row_delta = -1; row_delta <= 1; row_delta++)
            {
                for (int col_delta = -1; col_delta <= 1; col_delta++)
                {
                    xSumPixelRed += borderedImage[row + row_delta][col + col_delta].rgbtRed * kernelX[1 + row_delta][1 + col_delta];
                    xSumPixelGreen += borderedImage[row + row_delta][col + col_delta].rgbtGreen * kernelX[1 + row_delta][1 + col_delta];
                    xSumPixelBlue += borderedImage[row + row_delta][col + col_delta].rgbtBlue * kernelX[1 + row_delta][1 + col_delta];

                    ySumPixelRed += borderedImage[row + row_delta][col + col_delta].rgbtRed * kernelY[1 + row_delta][1 + col_delta];
                    ySumPixelGreen += borderedImage[row + row_delta][col + col_delta].rgbtGreen * kernelY[1 + row_delta][1 + col_delta];
                    ySumPixelBlue += borderedImage[row + row_delta][col + col_delta].rgbtBlue * kernelY[1 + row_delta][1 + col_delta];
                }
            }

            int redChannel = round(sqrt(pow(xSumPixelRed, 2) + pow(ySumPixelRed, 2)));
            if (redChannel > 255)
            {
                redChannel = 255;
            }
            resultImage[row - 1][col - 1].rgbtRed = redChannel;

            int greenChannel = round(sqrt(pow(xSumPixelGreen, 2) + pow(ySumPixelGreen, 2)));
            if (greenChannel > 255)
            {
                greenChannel = 255;
            }
            resultImage[row - 1][col - 1].rgbtGreen = greenChannel;

            int blueChannel = round(sqrt(pow(xSumPixelBlue, 2) + pow(ySumPixelBlue, 2)));
            if (blueChannel > 255)
            {
                blueChannel = 255;
            }
            resultImage[row - 1][col - 1].rgbtBlue = blueChannel;
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            image[row][col] = resultImage[row][col];
        }
    }

    return;
}
