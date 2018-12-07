package sth.app.student;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.app.exceptions.NoSuchDisciplineException;
import sth.app.exceptions.NoSuchProjectException;
import sth.SchoolManager;
import sth.exceptions.InvalidDisciplineSelectionException;
import sth.exceptions.ProjectDoesntExistException;
import sth.exceptions.ProjectNotOpenException;

/**
 * 4.4.1. Deliver project.
 */
public class DoDeliverProject extends Command<SchoolManager> {

	private Input<String> _discipline;
	private Input<String> _project;
	private Input<String> _submission;

    /**
    * @param receiver
    */
    public DoDeliverProject(SchoolManager receiver) {
        super(Label.DELIVER_PROJECT, receiver);
        _discipline = _form.addStringInput(Message.requestDisciplineName());
        _project = _form.addStringInput(Message.requestProjectName());
        _submission = _form.addStringInput(Message.requestDeliveryMessage());
    }

  	/** @see pt.tecnico.po.ui.Command#execute() */
  	@Override
	public final void execute() throws DialogException {
		_form.parse();
		try{
			_receiver.submitProject(_discipline.value(), _project.value(), _submission.value());
		}
		catch (InvalidDisciplineSelectionException e){ throw new NoSuchDisciplineException(_discipline.value());}
		catch (ProjectDoesntExistException e){ throw new NoSuchProjectException(_discipline.value(), _project.value());}
		catch (ProjectNotOpenException e){ throw new NoSuchProjectException(_discipline.value(), _project.value());}
  	}

}
