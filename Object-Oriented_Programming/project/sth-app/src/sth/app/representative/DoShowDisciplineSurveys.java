package sth.app.representative;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Display;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import sth.SchoolManager;
import sth.exceptions.InvalidDisciplineSelectionException;
import sth.app.exceptions.NoSuchDisciplineException;


/**
 * 4.5.6. Show discipline surveys.
 */
public class DoShowDisciplineSurveys extends Command<SchoolManager> {


	private Input<String> _disciplineName;
  /**
   * @param receiver
   */
  public DoShowDisciplineSurveys(SchoolManager receiver) {
    super(Label.SHOW_DISCIPLINE_SURVEYS, receiver);
	_disciplineName = _form.addStringInput(Message.requestDisciplineName());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException, NoSuchDisciplineException {
	_form.parse();
	try {
		for (String s: _receiver.showSurveyResultsRepresentative(_disciplineName.value()))
			_display = _display.addLine(s);
		_display.display();
	}  
	catch (InvalidDisciplineSelectionException e) {throw new NoSuchDisciplineException(e.getName());}
  }

}
