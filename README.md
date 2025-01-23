# HTTP-Server

This is a HTTP-Server where i will try to use functions pointers as a way to asign function calls to different routes

The plan is to have some sort of tree for the routes such that it is organised and fast
Then i want to be able to use a function such as, asignRouteFunction(char* route, middleware, function),
where the server will be able to just simply call that function when the client makes a request.
It would be similar to something like app.get("/route", function) in express.js .

I will also have to make some http response functions like a redirect, and sending off files ect.

This project is meant to be a learning experience, thus i want to try to avoid using online solutions
and instead i will try to implement everything myself and perhaps compare it to others later.
