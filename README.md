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
    curl -X POST http://localhost:8000/price_bond -d "face_value=1000&coupon_rate=0.05&periods=10&yield_to_maturity=0.04"
    ```

    **Example 2:**
    ```sh
    curl -X POST http://localhost:8000/price_bond -d "face_value=500&coupon_rate=0.03&periods=5&yield_to_maturity=0.02"
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

