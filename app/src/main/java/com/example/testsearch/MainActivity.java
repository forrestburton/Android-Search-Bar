package com.example.testsearch;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity implements AdapterView.OnItemClickListener {

    ListView lvUniversities;
    CustomAdapter universityAdapter;

    int images[] = {R.drawable.ucla_logo, R.drawable.ucsb_logo, R.drawable.yale_logo, R.drawable.uw_logo, R.drawable.stanford_logo};
    String names[] = {"UCLA", "UCSB", "Yale", "UW", "Stanford"};
    List<UniversityModel> universityList = new ArrayList<>();    //vector holding universities. Change to firebase for classes


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        lvUniversities = findViewById(R.id.lvUniversities);

        for (int i = 0; i < names.length; i++)   //Add each university to the vector
        {
            UniversityModel universityModel = new UniversityModel(names[i], images[i]);
            universityList.add(universityModel);
        }

        universityAdapter = new CustomAdapter(universityList, this);
        lvUniversities.setAdapter(universityAdapter);
        lvUniversities.setOnItemClickListener(this);
    }




    public class CustomAdapter extends BaseAdapter {

        private List<UniversityModel> universities;
        private List<UniversityModel> universitiesFiltered;
        private Context context;

        public CustomAdapter(List<UniversityModel> universities, Context context) {
            this.universities = universities;
            this.universitiesFiltered = universities;  //SUS
            this.context = context;
        }

        @Override
        public int getCount() {
            return universitiesFiltered.size();
        }

        @Override
        public Object getItem(int position) {
            return null;
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            View view = getLayoutInflater().inflate(R.layout.row_items, null);

            ImageView universityView = view.findViewById(R.id.universityView);
            TextView universityName = view.findViewById(R.id.universityName);

            universityView.setImageResource(universitiesFiltered.get(position).getImage());
            universityName.setText(universitiesFiltered.get(position).getName());


            return view;
        }
    }


    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {    //Recycler view onItemClick?
        String university = ((UniversityModel)parent.getItemAtPosition(position)).getName();
        Toast.makeText(getApplicationContext(), "Selected: " + university, Toast.LENGTH_SHORT).show();
    }


}