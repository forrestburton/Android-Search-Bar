package com.example.testsearch;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity implements AdapterView.OnItemClickListener {

    ListView lvUniversities;
    ArrayAdapter<String> universityAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        lvUniversities = findViewById(R.id.lvUniversities);
        List<String> universityList = new ArrayList<>();    //vector holding universities. Change to firebase
        universityList.add("UCLA");
        universityList.add("UCSB");
        universityList.add("Yale");
        universityList.add("Harvard");
        universityList.add("Stanford");

        universityAdapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, universityList);
        lvUniversities.setAdapter(universityAdapter);

        lvUniversities.setOnItemClickListener(this);
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        String university = parent.getItemAtPosition(position).toString();
        Toast.makeText(getApplicationContext(), "Selected: " + university, Toast.LENGTH_SHORT).show();
    }


}