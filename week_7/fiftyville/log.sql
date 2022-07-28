-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Find the correct crime scene report from the location of the crime.
SELECT *
  FROM crime_scene_reports
 WHERE street = "Humphrey Street";

-- Find the interview notes, based on the time of the interview and mention of bakery.
SELECT *
  FROM interviews
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND transcript LIKE "%bakery%";

-- Find the owners of license plates of cars, exiting the parking lot within ten minutes of the crime.
SELECT *
  FROM bakery_security_logs
       JOIN people
       ON people.license_plate = bakery_security_logs.license_plate
WHERE bakery_security_logs.year = 2021
   AND bakery_security_logs.month = 7
   AND bakery_security_logs.day = 28
   AND bakery_security_logs.hour = 10
   AND bakery_security_logs.minute >= 15
   AND bakery_security_logs.minute <= 25
   AND bakery_security_logs.activity = "exit";

-- Find atm_transactions, withdrawing some money before the crime and crosscheck with the owners of the license plates.
SELECT *
  FROM atm_transactions
       JOIN bank_accounts
       ON bank_accounts.account_number = atm_transactions.account_number

       JOIN people
       ON people.id = bank_accounts.person_id
 WHERE atm_transactions.year = 2021
   AND atm_transactions.month = 7
   AND atm_transactions.day = 28
   AND atm_transactions.atm_location = "Leggett Street"
   AND atm_transactions.transaction_type = "withdraw"
   AND people.id IN
       (SELECT people.id
         FROM bakery_security_logs
              JOIN people
              ON people.license_plate = bakery_security_logs.license_plate
       WHERE bakery_security_logs.year = 2021
          AND bakery_security_logs.month = 7
          AND bakery_security_logs.day = 28
          AND bakery_security_logs.hour = 10
          AND bakery_security_logs.minute >= 15
          AND bakery_security_logs.minute <= 25
          AND bakery_security_logs.activity = "exit");

-- Find calls on that day, lasting less than one minute and crosscheck with previous searches
SELECT *
  FROM phone_calls
       JOIN people
       ON people.phone_number = phone_calls.caller
 WHERE phone_calls.year = 2021
   AND phone_calls.month = 7
   AND phone_calls.day = 28
   AND phone_calls.duration <= 60
   AND people.id IN
       (SELECT people.id
         FROM atm_transactions
              JOIN bank_accounts
              ON bank_accounts.account_number = atm_transactions.account_number

              JOIN people
              ON people.id = bank_accounts.person_id
        WHERE atm_transactions.year = 2021
          AND atm_transactions.month = 7
          AND atm_transactions.day = 28
          AND atm_transactions.atm_location = "Leggett Street"
          AND atm_transactions.transaction_type = "withdraw"
          AND people.id IN
              (SELECT people.id
                FROM bakery_security_logs
                     JOIN people
                     ON people.license_plate = bakery_security_logs.license_plate
              WHERE bakery_security_logs.year = 2021
                 AND bakery_security_logs.month = 7
                 AND bakery_security_logs.day = 28
                 AND bakery_security_logs.hour = 10
                 AND bakery_security_logs.minute >= 15
                 AND bakery_security_logs.minute <= 25
                 AND bakery_security_logs.activity = "exit"));

-- Find the earliest flight the next day.
SELECT *
  FROM flights
       JOIN airports
       ON flights.destination_airport_id = airports.id
 WHERE flights.year = 2021
   AND flights.month = 7
   AND flights.day = 29;

-- Find the passengers on the earliest flight and crosscheck with the previous searches.
SELECT *
  FROM flights
       JOIN airports
       ON flights.destination_airport_id = airports.id

       JOIN passengers
       ON passengers.flight_id = flights.id

       JOIN people
       ON passengers.passport_number = people.passport_number
 WHERE flights.year = 2021
   AND flights.month = 7
   AND flights.day = 29
   AND flights.id = 36
   AND people.id IN
       (SELECT people.id
          FROM phone_calls
               JOIN people
               ON people.phone_number = phone_calls.caller
         WHERE phone_calls.year = 2021
           AND phone_calls.month = 7
           AND phone_calls.day = 28
           AND phone_calls.duration <= 60
           AND people.id IN
               (SELECT people.id
                 FROM atm_transactions
                      JOIN bank_accounts
                      ON bank_accounts.account_number = atm_transactions.account_number

                      JOIN people
                      ON people.id = bank_accounts.person_id
                WHERE atm_transactions.year = 2021
                  AND atm_transactions.month = 7
                  AND atm_transactions.day = 28
                  AND atm_transactions.atm_location = "Leggett Street"
                  AND atm_transactions.transaction_type = "withdraw"
                  AND people.id IN
                      (SELECT people.id
                        FROM bakery_security_logs
                             JOIN people
                             ON people.license_plate = bakery_security_logs.license_plate
                      WHERE bakery_security_logs.year = 2021
                         AND bakery_security_logs.month = 7
                         AND bakery_security_logs.day = 28
                         AND bakery_security_logs.hour = 10
                         AND bakery_security_logs.minute >= 15
                         AND bakery_security_logs.minute <= 25
                         AND bakery_security_logs.activity = "exit")));

-- Bruce is a match! Now find the accomplice by the receiver id.
SELECT *
  FROM people
 WHERE phone_number = "(375) 555-8161"