#include <stdio.h>
#include <time.h>

int isCouponExpired(const char *expiryDate)
{
    // This struct will hold the expiry date of a coupon.
    struct tm expiry = {0};

    // A variable called now with a type time_t
    time_t now;

    // A pointer called current, points to a structure that represents the current date and time.
    struct tm *current;

    // Reads the expiry date from a string called expiryDate in the format "YYYY-MM-DD" and stores the year, month, and day into the respective fields of the expiry structure.
    sscanf(expiryDate, "%d-%d-%d", &expiry.tm_year, &expiry.tm_mon, &expiry.tm_mday);

    // Adjusts the year field in the expiry structure. The tm_year field expects the number of years since 1900, so this line converts the full year (e.g., 2024) to the expected format by subtracting 1900.
    expiry.tm_year -= 1900; // Adjust year

    // Adjusts the month field in the expiry structure. The tm_mon field expects months in the range 0-11 (where 0 is January), so this line converts the month from the usual 1-12 range to 0-11 by subtracting 1.
    expiry.tm_mon -= 1;     // Adjust month

    // This function call gets the current time and stores it in the now variable.
    time(&now);

    // Converts the time stored in "now" to local time and stores the result in the current pointer. This pointer now points to a struct "tm" that represents the current date and time.
    current = localtime(&now);

    // Compares the expiry date with the current date. mktime converts the struct tm to time_t for comparison. difftime calculates the difference in seconds between the expiry date and the current date. If the result is less than 0, it means the expiry date is in the past.
    if (difftime(mktime(&expiry), mktime(current)) < 0)
    {
        return 1; // Expired
    }
    return 0; // Not expired
}