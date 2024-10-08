#ifndef __AtomAnimator_h
#define __AtomAnimator_h

class AtomAnimator
{
public:
  AtomAnimator(vtkActor* Actor, const vtkVector3d& StartPosition,
                const vtkVector3d& EndPosition)
    : Actor(Actor), StartPosition(StartPosition), EndPosition(EndPosition)
  {
  }

  AtomAnimator()
    : Actor(nullptr), StartPosition(0, 0, 0), EndPosition(0.5, 0.5, 0.5)
  {
  }

  ~AtomAnimator() = default;

  void SetActor(vtkActor* actor)
  {
    this->Actor = actor;
  }

  void SetStartPosition(const vtkVector3d& position)
  {
    this->StartPosition = position;
  }
  void SetEndPosition(const vtkVector3d& position)
  {
    this->EndPosition = position;
  }

  void AddObserversToCue(vtkAnimationCue* cue)
  {
    cue->AddObserver(vtkCommand::StartAnimationCueEvent, this,
                     &AtomAnimator::Start);
    cue->AddObserver(vtkCommand::EndAnimationCueEvent, this,
                     &AtomAnimator::End);
    cue->AddObserver(vtkCommand::AnimationCueTickEvent, this,
                     &AtomAnimator::Tick);
  }

private:
  //@{
  /**
   * These are callbacks that called when corresponding events are fired by the
   * cue (see AddObserversToCue)
   */
  void Start()
  {
    this->Actor->SetPosition(this->StartPosition.GetData());
  }

  void Tick(vtkObject* vtkNotUsed(caller), unsigned long vtkNotUsed(event),
            void* calldata)
  {
    vtkAnimationCue::AnimationCueInfo* info =
        reinterpret_cast<vtkAnimationCue::AnimationCueInfo*>(calldata);
    const double t = (info->AnimationTime - info->StartTime) /
        (info->EndTime - info->StartTime);
    vtkVector3d position =
        this->StartPosition + (this->EndPosition - this->StartPosition) * t;
    this->Actor->SetPosition(position.GetData());
  }

  void End()
  {
    this->Actor->SetPosition(this->EndPosition.GetData());
  }
  //@}

  vtkActor* Actor;
  vtkVector3d StartPosition;
  vtkVector3d EndPosition;
};

#endif