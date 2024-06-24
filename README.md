# BondPriceAPI

BondPriceAPI is a RESTful API built in C using the Mongoose library to calculate bond prices based on user-provided parameters.

## Project Structure

|-- include/

| |-- bond_pricing.h

| |-- client.h

|-- src/

| |-- bond_pricing.c

| |-- client.c

| |-- main.c

|-- mongoose.c

|-- mongoose.h

|-- Makefile



## Prerequisites

- GCC (GNU Compiler Collection)
- Mongoose Library

## Installation

1. **Clone the repository:**
    ```sh
    git clone https://github.com/your_username/BondPriceAPI.git
    cd BondPriceAPI
    ```

2. **Compile the project:**
    ```sh
    make
    ```

## Usage

1. **Run the server:**
    ```sh
    ./main
    ```

    The server will start and listen on `http://0.0.0.0:8000`.

2. **Calculate bond price:**

    You can use `curl` to send a POST request to calculate the bond price.

    **Example 1:**
    ```sh
    curl -X POST http://localhost:8000/price_bond -d "face_value=100&coupon_rate=0.05&periods=10&yield_to_maturity=0.04"
    ```

    **Example 2:**
    ```sh
    curl -X POST http://localhost:8000/price_bond -d "face_value=500&coupon_rate=0.03&periods=5&yield_to_maturity=0.02"
    ```
3. **Fetch SOFR rates:**

    You can use `curl` to send a GET request to fetch the latest SOFR rates.

    **Example:**
    ```sh
    curl -X GET http://localhost:8000/fetch_sofr_rates
    ```

## API Endpoint

### `/price_bond`

- **Method:** POST
- **Description:** Calculate the price of a bond.
- **Parameters:**
  - `face_value`: The face value of the bond.
  - `coupon_rate`: The annual coupon rate of the bond.
  - `periods`: The number of periods until maturity.
  - `yield_to_maturity`: The yield to maturity of the bond.
- **Response:** JSON object containing the bond price.

**Request:**
```sh
curl -X POST http://localhost:8000/price_bond -d "face_value=1000&coupon_rate=0.05&periods=10&yield_to_maturity=0.04"


**Example Response**

{ "bond_price": 108.10900000 }

### `/fetch_sofr_rates`

- **Method:** GET
- **Description:** Fetch the latest SOFR rates from the FRED API
- **Parameters:** None
- **Response:** JSON object containing an array of SOFR rates.

**Request:**

```sh
curl -X GET http://localhost:8000/fetch_sofr_rates

**Example Response**

{
    "sofr_rates": [
        5.32000000,
        5.31000000,
        5.31000000,
        ...
    ]
}

**Request:**
```
curl -X POST http://localhost:8000/price_swap -d "notional=1000000&fixed_rate=0.02&periods=10&floating_rates=0.015,0.016,0.017,0.018,0.019,0.02,0.021,0.022,0.023,0.024"
```

**Example Response**
{"swap_price": 5000.00000000}

**Running Tests*

Run the below:

```
make test
```

