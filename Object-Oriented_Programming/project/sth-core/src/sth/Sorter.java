package sth;

import java.util.Comparator;
import java.text.Collator;
import java.util.Locale;
import java.io.Serializable;

public class Sorter implements Comparator<String>, Serializable{
	public int compare(String s1, String s2){
		Collator collator = Collator.getInstance(Locale.getDefault());
		return collator.compare(s1, s2);
	}
}
