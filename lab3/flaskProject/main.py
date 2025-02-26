from flask import request, Flask, jsonify, Response
app = Flask(__name__)
@app.route('/')
def hello_world():
    return 'Hello World'
@app.route('/salut/<int:numar>')
def salut_n(numar):
    return "Hello " + str(numar)
# @app.route('/call_me')
# def call_me():
#     payload = request.get_json(silent=True)
#     if not payload:
#         # Error handling
#         return Response(status=400)
#
#     return jsonify({'status': 'ok'}), 201

# film_type = {
#     "id": 1,
#     "nume": "Star Wars 1 - Yoda Version"
# }

movies = []
current_id = 1

@app.route('/movies', methods=['GET'])
def get_movies():
    return jsonify(movies), 200


@app.route('/movies', methods=['POST'])
def add_movie():
    global current_id
    if not request.json or 'nume' not in request.json:
        return Response(status=400)

    movie = {
        'id': current_id,
        'nume': request.json['nume']
    }

    movies.append(movie)
    current_id += 1
    return jsonify(movie), 201

@app.route('/movie/<int:movie_id>', methods=['PUT'])
def update_movie(movie_id):
    if not request.json or 'nume' not in request.json:
        return Response(status=400)

    movie = next((m for m in movies if m['id'] == movie_id), None)
    if movie is None:
        return Response(status=400)

    movie['nume'] = request.json['nume']
    return jsonify(movie), 200

@app.route('/movie/<int:movie_id>', methods=['GET'])
def get_movie(movie_id):
    movie = next((m for m in movies if m['id'] == movie_id), None)
    if movie is None:
        return Response(status=400)

    return jsonify(movie), 200

@app.route('/movie/<int:movie_id>', methods=['DELETE'])
def delete_movie(movie_id):
    global movies
    movie = next((m for m in movies if m['id'] == movie_id), None)
    if movie is None:
        return Response(status=400)

    movies = [m for m in movies if m['id'] != movie_id]
    return '', 200

@app.route('/reset', methods=['DELETE'])
def reset():
    global movies, current_id
    movies = []
    current_id = 1
    return '', 200

if __name__ == '__main__':
    app.run("127.0.0.1", port=5000, debug=False)
