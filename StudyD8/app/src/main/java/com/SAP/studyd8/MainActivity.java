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

public class MainActivity extends AppCompatActivity {
    TextView welcomeText, unverifiedText;
    FirebaseAuth fAuth;
    Button logoutButton,resendEmailButton;

    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        startActivity(new Intent(getApplicationContext(), Search.class));



        /*
        try {
            welcomeText = findViewById(R.id.welcomeText);
            logoutButton = findViewById(R.id.logoutButton);
            unverifiedText = findViewById(R.id.unverifiedText);
            resendEmailButton = findViewById(R.id.resendVerButton);


            fAuth = getInstance();
            final FirebaseUser user = fAuth.getCurrentUser();


            if (fAuth.getCurrentUser() == null) {
                startActivity(new Intent(getApplicationContext(), Login.class));
                finish();
            }
            if (!user.isEmailVerified()) {
                unverifiedText.setVisibility(View.VISIBLE);
                resendEmailButton.setVisibility(View.VISIBLE);
                resendEmailButton.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        user.sendEmailVerification().addOnSuccessListener(new OnSuccessListener<Void>() {
                            @Override
                            public void onSuccess(Void aVoid) {
                                Toast.makeText(getApplicationContext(), "Please check your email", Toast.LENGTH_SHORT).show();
                            }
                        }).addOnFailureListener(new OnFailureListener() {
                            @Override
                            public void onFailure(@NonNull Exception e) {
                                Toast.makeText(getApplicationContext(), "Unable to send verification email", Toast.LENGTH_SHORT).show();
                            }
                        });
                    }
                });
            }

            logoutButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    FirebaseAuth.getInstance().signOut();
                    startActivity(new Intent(getApplicationContext(), Login.class));
                    finish();
                }
            });
            assert user != null;
            welcomeText.setText("hello " + user.getEmail());

        } catch (Exception e) {
//            Toast.makeText(getApplicationContext(),"Unfortunately an error occurred "+e.getMessage(),Toast.LENGTH_LONG).show();
        }
        */
    }


    /*
    //NEW CODE
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.my_menu, menu);

        MenuItem menuItem = menu.findItem(R.id.search_icon);
        SearchView searchView = (SearchView) menuItem.getActionView();
        searchView.setQueryHint("Search for University");

        searchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String s) {
                return false;
            }

            @Override
            public boolean onQueryTextChange(String s) {
                universities.getFilter().filter(s);
                return true;
            }
        });

        return super.onCreateOptionsMenu(menu);
    }
     */
}

//END