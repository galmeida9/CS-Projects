package sth.app.student;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.app.exceptions.NoSuchDisciplineException;
import sth.app.exceptions.NoSuchProjectException;
import sth.app.exceptions.NoSurveyException;
import sth.SchoolManager;
import sth.exceptions.InvalidDisciplineSelectionException;
import sth.exceptions.ProjectDoesntExistException;
import sth.exceptions.SurveyDoesntExistException;

/**
 * 4.4.2. Answer survey.
 */
public class DoAnswerSurvey extends Command<SchoolManager> {

	private Input<String> _discipline;
	private Input<String> _project;
	private Input<String> _comment;
	private Input<Integer> _hours;

	/**
	* @param receiver
	*/
	public DoAnswerSurvey(SchoolManager receiver) {
		super(Label.ANSWER_SURVEY, receiver);
        _discipline = _form.addStringInput(Message.requestDisciplineName());
        _project = _form.addStringInput(Message.requestProjectName());
		_hours = _form.addIntegerInput(Message.requestProjectHours());
        _comment = _form.addStringInput(Message.requestComment());
	}

	/** @see pt.tecnico.po.ui.Command#execute() */
	@Override
	public final void execute() throws DialogException {
		_form.parse();
		try{
			_receiver.submitSurvey(_discipline.value(), _project.value(), _hours.value(), _comment.value());	
		}
		catch (InvalidDisciplineSelectionException e){ throw new NoSuchDisciplineException(_discipline.value());}
		catch (ProjectDoesntExistException e){ throw new NoSuchProjectException(_discipline.value(), _project.value());}
		catch (SurveyDoesntExistException e){throw new NoSurveyException(_discipline.value(), _project.value());}
	}

}
