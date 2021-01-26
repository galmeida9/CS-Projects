package sth;

import java.io.Serializable;
import sth.exceptions.OperationUnsupportedException;

class Administrative extends Person implements Serializable{


	public Administrative(String name, String telephone, int id){
		super(name, telephone, id);
	}

	public void addDiscipline(Discipline d) throws OperationUnsupportedException{
		throw new OperationUnsupportedException();
	}
	public String getDisciplines(){
		return "";
	}
	public void addCourse(Course c) throws OperationUnsupportedException{
		throw new OperationUnsupportedException();
	}
	public boolean containsDiscipline(String name) throws OperationUnsupportedException{
		throw new OperationUnsupportedException();
	}
	public Discipline getDiscipline(String name) throws OperationUnsupportedException{
		throw new OperationUnsupportedException();
	}
}

