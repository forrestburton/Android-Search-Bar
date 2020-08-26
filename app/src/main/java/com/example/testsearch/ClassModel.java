package com.example.testsearch;

public class ClassModel {

    private String name;
    private String classCode;

    public ClassModel(String classCode, String name) {
        this.name = name;
        this.classCode = classCode;
    }

    //getters and setters
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getClassCode() {
        return classCode;
    }

    public void setClassCode(String classCode) {
        this.classCode = classCode;
    }
}
