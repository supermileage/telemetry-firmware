class JsonMaker {
    public:
        //Constructor
        JsonMaker();

        // Clear the char buffer and initialize
        void clear();

        //Add id and value if id not present, otherwise replace value to the id
        void add(String id, int value);

        //Get the JSON string
        String get();
    private:

};