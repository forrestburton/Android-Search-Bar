package com.example.testsearch;

public class UniversityModel {

    private String name;
    private int image;

    public UniversityModel(String name, int image) {
        this.name = name;
        this.image = image;
    }

    //GETTERS and SETTERS
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getImage() {
        return image;
    }

    public void setImage(int image) {
        this.image = image;
    }
}
