/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Endpoint;

import java.io.IOException;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;
import javax.websocket.OnClose;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;

/**
 *
 * @author Jesse
 */
@ServerEndpoint("/endpoint")
public class Endpoint {

    private static final Set<Session> sessions = Collections.synchronizedSet(new HashSet<Session>());
    
    @OnOpen
    public void onOpen(Session session){
        sessions.add(session);
        System.out.println("New Client: " + session.getId() + " Connected.");
    }
    
    @OnMessage
    public String onMessage(String message) {
       // System.out.println(message);
        for(Session s : sessions){
            try {
                s.getBasicRemote().sendText(message);
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
        return null;
    }
    
    @OnClose
    public void onClose(Session session){
        sessions.remove(session);
        System.out.println("Session: " + session.getId() + " closed.");
        
    }
    
}
