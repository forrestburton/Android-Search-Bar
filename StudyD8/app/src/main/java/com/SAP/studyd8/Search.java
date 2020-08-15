package com.SAP.studyd8;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.SearchView;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;
import java.util.ArrayList;

import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

import static com.google.firebase.auth.FirebaseAuth.getInstance;

public class Search extends AppCompatActivity implements AdapterView.OnItemClickListener {
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
        Toast.makeText(getApplicationContext(), "Clicked: " + university, Toast.LENGTH_SHORT).show();
    }
}