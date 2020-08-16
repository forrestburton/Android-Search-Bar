package com.example.testsearch;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.Filter;
import android.widget.Filterable;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.SearchView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity /*implements AdapterView.OnItemClickListener*/ {

    ListView lvUniversities;
    CustomAdapter universityAdapter;

    int images[] = {R.drawable.ucla_logo, R.drawable.ucsb_logo, R.drawable.yale_logo, R.drawable.uw_logo, R.drawable.stanford_logo};
    String names[] = {"University Of California, Los Angeles", "University of California, Berkeley", "Yale", "University of Washington", "Stanford"};
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
        //lvUniversities.setOnItemClickListener(this);
    }


    //IMPLEMENTING SEARCH VIEW
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu, menu);

        MenuItem menuItem = menu.findItem(R.id.search_view);
        SearchView searchView = (SearchView) menuItem.getActionView();
        searchView.setQueryHint("Search...");

        searchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String s) {
                return false;
            }

            @Override
            public boolean onQueryTextChange(String newText) {
                universityAdapter.getFilter().filter(newText);

                return true;
            }
        });

        return true;
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        int id = item.getItemId();

        if (id == R.id.search_view) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }



    //Custom Adapter for UniversityModel Class instead of just String
    public class CustomAdapter extends BaseAdapter implements Filterable {

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

        //Filtering search results
        @Override
        public Filter getFilter() {
            Filter filter = new Filter() {
                @Override
                protected FilterResults performFiltering(CharSequence constraint) {
                    FilterResults filterResults = new FilterResults();

                    if(constraint == null || constraint.length() == 0) {
                        filterResults.count = universities.size();
                        filterResults.values = universities;
                    }
                    else {
                        String searchStr = constraint.toString().toLowerCase();
                        List<UniversityModel> resultData = new ArrayList<>();

                        for (UniversityModel universityModel:universities) {
                            if(universityModel.getName().toLowerCase().contains(searchStr)) {
                                resultData.add(universityModel);
                            }
                        }

                        filterResults.count = resultData.size();
                        filterResults.values = resultData;
                    }

                    return filterResults;
                }

                @Override
                protected void publishResults(CharSequence constraint, FilterResults results) {

                    universitiesFiltered = (List<UniversityModel>) results.values;

                    notifyDataSetChanged();
                }
            };

            return filter;
        }
    }

    /*
    //When an list item is clicked
    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {    //Recycler view onItemClick?
        String university = ((UniversityModel)parent.getItemAtPosition(position)).getName();
        Toast.makeText(getApplicationContext(), "Selected: " + university, Toast.LENGTH_SHORT).show();
    }
    */

}