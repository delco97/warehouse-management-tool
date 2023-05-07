# Warehouse Management Tool

This project is a command-line tool written in C that allows users to manage products within a warehouse. The list of products is stored in the "Products.bin" file, and the tool allows users to order or sell products by updating the "Products.bin" file accordingly. Each product has a value that identifies the minimum quantity of pieces that must be in stock (critical quantity). If the stock falls below this value, an order must be placed. Additionally, this value is used to determine the maximum quantity of pieces for each product, which is equal to the minimum quantity multiplied by 5.

- create-product-list: Creates a new list of products and stores it in the "Products.bin" file.
- warehouse-management-tool: Manages the products within the warehouse using Products.bin in the local folder as the source of data.

## Requirements

To build and run the code, you will need to have the following software installed:

- Docker: Download and install Docker from the official website https://www.docker.com/get-started.
- Docker Compose: Download and install Docker Compose from the official website https://docs.docker.com/compose/install/.

## How to Build and Run

To build and run the code, follow these steps:

1. Download the source code from the project's GitHub repository: https://github.com/delco97/warehouse-management-tool.
2. Open the terminal and navigate to the project directory.
3. Build the project using GCC:

```bash
docker-compose run --rm program gcc -o ./build/create-product-list ./create-product-list/main.c
docker-compose run --rm program gcc -o ./build/warehouse-management-tool ./warehouse-management-tool/main.c
```

4. Run the project:

```bash
docker-compose run --rm program ./build/create-product-list
docker-compose run --rm program ./build/warehouse-management-tool
```

## Contributing

This project was created to practice C programming and is not intended for production use. However, if you want to contribute to it, feel free to fork the repository and make your changes. You can then submit a pull request with your changes for review.

## License

This project is licensed under the MIT License - see the `LICENSE` file for details.