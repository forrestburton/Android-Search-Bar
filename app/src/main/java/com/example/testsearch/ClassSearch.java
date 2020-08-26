package com.example.testsearch;

import android.content.Context;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Filter;
import android.widget.Filterable;
import android.widget.ListView;
import android.widget.SearchView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;
import java.util.List;

public class ClassSearch extends AppCompatActivity {

    ListView lvClasses;
    CustomAdapter classAdapter;

    String codes[] = {"1111", "2222", "7777"};
    String names[] = {"CS 31", "CS 32", "PHIL 7"};
    List<ClassModel> classList = new ArrayList<>();    //Change to firebase for classes


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_class_search);  //change xml file (activity_main)

        lvClasses = findViewById(R.id.lvClasses);    //need xml file  (lvUniversities)

        for (int i = 0; i < names.length; i++) {
            ClassModel classModel = new ClassModel(codes[i], names[i]);
            classList.add(classModel);
        }

        classAdapter = new CustomAdapter(classList, this);
        lvClasses.setAdapter(classAdapter);
    }


    //IMPLEMENTING SEARCH VIEW
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu, menu);

        MenuItem menuItem = menu.findItem(R.id.search_view);
        SearchView searchView = (SearchView) menuItem.getActionView();
        searchView.setQueryHint("Search by class code");

        searchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String s) {
                return false;
            }

            @Override
            public boolean onQueryTextChange(String newText) {
                classAdapter.getFilter().filter(newText);

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

        private List<ClassModel> classes;
        private List<ClassModel> classesFiltered;
        private Context context;

        public CustomAdapter(List<ClassModel> classes, Context context) {
            this.classes = classes;
            this.classesFiltered = classes;  //SUS
            this.context = context;
        }

        @Override
        public int getCount() {
            return classesFiltered.size();
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
        public View getView(final int position, View convertView, ViewGroup parent) {
            View view = getLayoutInflater().inflate(R.layout.class_row_items, null);

            TextView classCode = view.findViewById(R.id.classCode);  //change in xml file (universityView)
            TextView className = view.findViewById(R.id.className);  //change in xml file (className)

            classCode.setText(classesFiltered.get(position).getClassCode());
            className.setText(classesFiltered.get(position).getName());

            view.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                   // String currentUniversity = classesFiltered.get(position).getName();
                }
            });

            return view;
        }

        //Filtering search results
        @Override
        public Filter getFilter() {
            Filter filter = new Filter() {
                @Override
                protected FilterResults performFiltering(CharSequence constraint) {
                    FilterResults filterResults = new FilterResults();

                    if (constraint == null || constraint.length() == 0) {
                        filterResults.count = classes.size();
                        filterResults.values = classes;
                    } else {
                        String searchStr = constraint.toString().toLowerCase();
                        List<ClassModel> resultData = new ArrayList<>();

                        for (ClassModel classModel : classes) {
                            if (classModel.getClassCode().toLowerCase().contains(searchStr)) {
                                resultData.add(classModel);
                            }
                        }

                        filterResults.count = resultData.size();
                        filterResults.values = resultData;
                    }

                    return filterResults;
                }

                @Override
                protected void publishResults(CharSequence constraint, FilterResults results) {

                    classesFiltered = (List<ClassModel>) results.values;

                    notifyDataSetChanged();
                }
            };

            return filter;
        }
    }
}