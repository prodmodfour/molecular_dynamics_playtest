To add:
    - On the fly setting modification and multiple species interaction (2 weeks)
        - Be able to change the settings of the simulation at each timestep
        - Be able to add atoms at any timestep (already implemented)
        - Be able to click on an atoms and get information on it
        - Be able to remove atoms at any timestep by clicking and selecting remove
        - Be able to modify the properties of an atom by clicking and selecting modify
        - Have multiple types of atoms interacting with each other, not just Cu- Cu

        Relevant KSBs:

            K13: Principles of data analysis for digital and technology solutions.

            K57: Approaches to data processing and storage, database systems, data warehousing, and online analytical processing.

            S4: Initiate, design, code, test, and debug a software component for a digital and technology solution.

            S10: Initiate, design, implement, and debug a data product for a digital and technology solution.

            S11: Determine and use appropriate data analysis techniques.



    - Domain Decomposition (2 weeks)
        - The system, at each timstep, will be split into domains (domain number and balancing will be handled automatically by open mp, much simpler than mpi)
        - Atoms will be assigned to domains based on their position
        - Atoms will be assigned to cells based on their position
        - Domains will send boundary atoms to neighbouring domains
        - We don't have a periodic boundary condition, so this is much simpler than the mpi version that I did last year
        - This will massively increase the number of atoms that we can simulate 

        Relevant KSBs:

            K6: Approaches and techniques used throughout the digital and technology solution lifecycle.

            K55: Data formats, structures, architectures, and data delivery methods.

            S11: Determine and use appropriate data analysis techniques.

            S9: Apply relevant security and resilience techniques.

            S13: Plan, design, and manage simple computer networks with an overall focus on services and capabilities.

    - Documentation Creation (1 week)
        Relevant KSBs:

            K18: Reporting techniques and synthesizing information.

            B4: Commits to continuous professional development.





    - Initial Set up GUI (3 weeks)
        During initial set up
            - Be able to click on an atom and get information on it
            - Be able to click on an atom and edit its properties
            - Be able to move selections of atoms using the mouse
            - Be able to resize structures using the mouse
            - Be able to click on a structure and get information on it
            - Be able to click on a structure and edit its properties

        Relevant KSBs:

            K12: Role of data management systems within digital and technology solutions.

            S4: Initiate, design, code, test, and debug software components.

            S10: Implement and debug data products for digital technology solutions.

            S49: Apply different types of data analysis to drive improvements for specific business problems.


    - Documentation update (1 week)


    - System analysis and context creation (6 weeks)
        - During each timestep, additional functions will run that will analyse the system and look for specific patterns
        - Upon finding these patterns, that timestep and sections of the system will have strings of text added to them that will be used to create context for the chatbot\
        - Examples of patterns:
            - Crystalinity
            - Phase transitions
                - Localised or entire system
            - Cratering
            - Grain Boundary Formation and Migration
                - Grain boundaries can emerge in polycrystalline materials, where different crystallite orientations meet.
                - Tracking the motion or evolution of these boundaries can provide insights into recrystallization, coarsening, or mechanical strength. 
            - Nucleation Events
                - Nucleation of new phases (e.g., vapor bubbles in liquids or crystalline phases in melts) often begins with small clusters forming, then growing under favorable thermodynamic conditions.
                - Detecting these events can reveal the onset of phase transitions.   
            - Sputtering
                - Sputtering is the ejection of atoms from a surface due to high-energy particle bombardment.
                - This can be used to study the surface structure and dynamics of materials.

        Relevant KSBs:

            K57: Approaches to data processing and storage, database systems, data warehousing, and online analytical processing.

            S11: Determine and use appropriate data analysis techniques.



    - Documentation update (1 week)
    
    - Extended event scripting (2 Weeks)
        - We already have a system that allows us to run events at specific timesteps, but we need to extend this to allow for more complex events
        - These events will be triggered by the chatbot, and will be able to modify the system in a variety of ways
        - Examples of scripted events:
            - Changing the thermostat
                - Can be applied to the sections or the entire system.
            - Forcing a phase transition
            - Creating a crater using an impact
            - Introducing doping atoms
                Adding new species into specified regions at certain timesteps to study alloy formation or impurity effects.
            - Creating grain boundaries 
        Relevant KSBs:

            K20: Sustainable development approaches in digital and technology solutions.

            K55: Data formats, structures, architectures, and data delivery methods.

            S10: Debug data products.

            S12: Manage infrastructure solutions in an organizational context.


            

    - Documentation update (1 week)

    - Chatbot (Can't estimate)
        - Create a chatbot that can interact with the system and the user
        - Allows the user to interact with the simulation in an inquisitive way. The chatbot will be able to encourage the user's curiosity and guide them through the simulation.
        - The chatbot will be able to answer questions about the system
        - The chatbot will be able to trigger events using the scripted events system
        - The chatbot will be able to use the context created by the system analysis and context creation system to answer questions
        - The chatbot will be able to use the context to suggest changes to the system
        - The chatbot will be able to use the context to suggest experiments to the user

        Relevant KSBs:
            K55: Data formats, structures, architectures, and data delivery methods.

            B1: Strong work ethic and commitment to high standards.
            
            B7: Maintains awareness of trends and innovations.

            S11: Determine and use appropriate data analysis techniques.

            K12: Role of data management systems within digital and technology solutions.

            S4: Initiate, design, code, test, and debug software components.

            S10: Implement and debug data products for digital technology solutions.

            S49: Apply different types of data analysis to drive improvements for specific business problems.
            
            K14: A range of quantitative and qualitative data gathering methods and how to appraise and select the appropriate method.

    - Documentation update (1 week)

    - Interactive lessons (Can't estimate)
        - Using the chatbot, the scriptinng system as well as the system analysis and context creation system, create a system that can teach lessons to the user

        - Examples of lessons:

            - Introduction to Molecular Dynamics Basics

                Objective: Teach users how to set up a simple MD simulation (e.g., Lennard-Jones particles).
                Interactions:
                The chatbot walks the user through choosing a potential and setting initial conditions.
                Users can visualize atom positions and velocities, then tweak parameters (temperature, density) to see how the system evolves.
                Automated Analysis:
                The system tracks changes in potential energy and temperature, displaying graphs in real time.
                The chatbot points out when equilibrium is reached 
                Chatbot can also suggest changes to the system to help the user reach equilibrium faster


            - Phase Transition Demonstration

                Objective: Show how varying temperature or pressure can induce phase changes (e.g., solid to liquid).
                Interactions:
                The user gradually increases the temperature or applies pressure.
                A scripted event can “force” a phase transition at a critical timestep
                Automated Analysis:
                The system identifies crystalline order or local structural changes.



            - Crack Formation and Propagation
                Objective: Visualize how a material responds to stress or strain.
                Interactions:
                The user applies a tensile load or compression in the simulation structure.
                At chosen timesteps, a scripted event artificially creates or extends a microcrack.
                Automated Analysis:
                The system monitors stress-strain curves and identifies crack initiation points.
                The chatbot explains crack propagation and fracture mechanics concepts.


            - Heat Shock or Localized Heating

                Objective: Learn how localized energy injections (e.g., laser pulses) affect materials.
                Interactions:
                The chatbot prompts the user to apply a sudden temperature spike in a localized region using scripted events.
                Users see how the hot region interacts with neighboring cooler regions, potentially causing local phase changes.
                Automated Analysis:
                Automatic detection of localized melting or structural damage.
                Context generation that details how energy dissipates and the effect on overall system kinetics.


            - Alloy Formation and Mixing

                Objective: Illustrate how different atomic species can mix or separate under given conditions.
                Interactions:
                The user sets up a two-species system (e.g., Cu and Ni) at different initial concentrations.
                The chatbot can suggest doping events or concentration changes at certain timesteps.
                Automated Analysis:
                Chatbot explains how thermodynamics drive mixing or separation in alloys.

        Relevant KSBs:

            K3: Digital technology solution lifecycle.

            S11: Data analysis techniques for driving improvements.

            S49: Apply different types of data analysis to drive improvements for specific business problems.
            
            K14: A range of quantitative and qualitative data gathering methods and how to appraise and select the appropriate method.

            S4: Initiate, design, code, test, and debug software components.

            S10: Implement and debug data products for digital technology solutions.


    - Documentation update (1 week)

    
