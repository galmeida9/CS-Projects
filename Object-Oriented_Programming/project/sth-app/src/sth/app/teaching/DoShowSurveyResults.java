package sth.app.teaching;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Display;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.SchoolManager;
import sth.app.exceptions.NoSuchProjectException;
import sth.app.exceptions.NoSurveyException;
import sth.exceptions.SurveyDoesntExistException;
import sth.exceptions.ProjectDoesntExistException;
import sth.exceptions.InvalidDisciplineSelectionException;
import sth.app.exceptions.NoSuchDisciplineException;

/**
 * 4.3.5. Show survey results.
 */
public class DoShowSurveyResults extends Command<SchoolManager> {


	private Input<String> _disciplineName;
	private Input<String> _projectName;
  /**
   * @param receiver
   */
  public DoShowSurveyResults(SchoolManager receiver) {
    super(Label.SHOW_SURVEY_RESULTS, receiver);
	_disciplineName = _form.addStringInput(Message.requestDisciplineName());
	_projectName = _form.addStringInput(Message.requestProjectName());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException, NoSuchDisciplineException, NoSurveyException, NoSuchProjectException {
	_form.parse();
	try {
		_display = _display.addLine(_receiver.showSurveyResults(_disciplineName.value(), _projectName.value()));
		_display.display();
	}  
	catch (SurveyDoesntExistException e) {throw new NoSurveyException(e.getDiscipline(), e.getProject());}
	catch (ProjectDoesntExistException e) {throw new NoSuchProjectException(e.getDiscipline(), e.getProject());}
	catch (InvalidDisciplineSelectionException e) {throw new NoSuchDisciplineException(e.getName());}
  }

}
